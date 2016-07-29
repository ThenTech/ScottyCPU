/**
*	Synchronous component with combined Signals and Slots functionality.
* 		Loosely based on https://github.com/winglot/Signals
*/
#ifndef SYNCHROTRONCOMPONENT_HPP
#define SYNCHROTRONCOMPONENT_HPP

#include <iostream> // For testing for now

#include <bitset>
#include <set>
#include <initializer_list>
#include <mutex>

namespace Synchrotron {

    /** \brief Mutex class to lock the current working thread.
     */
	class Mutex {
		private:
			std::mutex m_mutex;
		public:
			Mutex() 				{}
			Mutex(const Mutex&) 	{}
			virtual ~Mutex() 		{}
			virtual void lock()		{ m_mutex.lock();		}
			virtual void unlock()	{ m_mutex.unlock();		}
	};

	/**	\brief
	 *	Creating a new LockBlock(this) locks the current thread,
	 *	while leaving the scope conveniently unlocks the thread.
	 */
	class LockBlock {
		public:
			Mutex *m_mutex;
			LockBlock(Mutex *mtx)
				: m_mutex(mtx)		{ m_mutex->lock();		}
			~LockBlock()			{ m_mutex->unlock();	}
	};

	/** \brief
	 *	SynchrotronComponent is the base for all components,
	 *	offering in and output connections to other SynchrotronComponent.
	 *
	 *	\param	bit_width
	 *		This template argument specifies the width of the internal bitset state.
     */
	template <size_t bit_width>
	class SynchrotronComponent : public Mutex {
		protected:
			/**	\brief
			 *	The current internal state of bits in this component (default output).
			 */
			std::bitset<bit_width> state;

		private:
			/**	\brief
			 *	**Slots == outputs**
			 *
			 *		Emit this.signal to subscribers in slotOutput.
			 */
			std::set<SynchrotronComponent*> slotOutput;

			/**	\brief
			 *	**Signals == inputs**
			 *
			 *		Receive tick()s from these subscriptions in signalInput.
			 */
			std::set<SynchrotronComponent*> signalInput;

            /**	\brief	Connect a new slot s:
             *		* Add s to this SynchrotronComponent's outputs.
             *		* Add this to s's inputs.
             *
             *	\param	s
			 *		The SynchrotronComponent to connect.
             */
			inline void connectSlot(SynchrotronComponent* s) {
				//LockBlock lock(this);

				this->slotOutput.insert(s);
				s->signalInput.insert(this);
			}

            /**	\brief	Disconnect a slot s:
             *		* Remove s from this SynchrotronComponent's outputs.
             *		* Remove this from s's inputs.
             *
             *	\param	s
			 *		The SynchrotronComponent to disconnect.
             */
			inline void disconnectSlot(SynchrotronComponent* s) {
				//LockBlock lock(this);

				this->slotOutput.erase(s);
				s->signalInput.erase(this);
			}

		public:
            /** \brief	Default constructor
             *
             *	\param	initial_value
			 *		The initial state of the internal bitset.
			 *	\param	bit_width
			 *		The size of the internal width of the bitset.
             */
			SynchrotronComponent(size_t initial_value = 0) : state(initial_value) {}

			/**	\brief **[Thread safe]**
			 *	Copy constructor
			 *	*	Duplicates signal subscriptions (inputs)
			 *	*	Optionally also duplicates slot connections (outputs)
			 *
			 *	\param	sc const
			 *		The other SynchrotronComponent to duplicate the connections from.
			 *	\param	duplicateAll_IO
			 *		Specifies whether to only copy inputs (false) or outputs as well (true).
			 */
			SynchrotronComponent(const SynchrotronComponent& sc, bool duplicateAll_IO = false) : SynchrotronComponent() {
				LockBlock lock(this);

				// Copy subscriptions
				for(auto& sender : sc.signalInput) {
					sender->connectSlot(this);
				}

				if (duplicateAll_IO) {
					// Copy subscribers
					for(auto& connection : sc.slotOutput) {
						this->connectSlot(connection);
					}
				}
			}

			/**	\brief
			 *	Connection constructor
			 *	*	Adds signal subscriptions from inputList
			 *	*	Optionally adds slot subscribers from outputList
			 *
			 *	\param	inputList
			 *		The list of SynchrotronComponents to connect as input.
			 *	\param	outputList
			 *		The list of SynchrotronComponents to connect as output..
			 */
			SynchrotronComponent(std::initializer_list<SynchrotronComponent*> inputList,
								 std::initializer_list<SynchrotronComponent*> outputList = {} )
									: SynchrotronComponent() {
				this->addInput(inputList);
				this->addOutput(outputList);
			}

			/** \brief	**[Thread safe]** Default destructor
			 *
			 *		When called, will disconnect all in and output connections to this SynchrotronComponent.
             */
			~SynchrotronComponent() {
				LockBlock lock(this);

				// Disconnect all Slots
				for(auto& connection : this->slotOutput) {
					connection->signalInput.erase(this);
					//delete connection; //?
				}

				// Disconnect all Signals
				for(auto &sender: this->signalInput) {
					sender->slotOutput.erase(this);
				}

				this->slotOutput.clear();
				this->signalInput.clear();
			}

            /**	\brief	Gets this SynchrotronComponent's bit width.
             *
             *	\return	size_t
             *      Returns the bit width of the internal bitset.
             */
			inline size_t getBitWidth() {
				return bit_width;
			}

//			/* No real use since function cannot be called with different size SynchrotronComponents */
//			/* Maybe viable when SynchrotronComponent has different in and output sizes */
//			/*	\brief	Compare this bit width to that of other.
//             *
//             *	\param	other
//			 *		The other SynchrotronComponent to check.
//			 *
//             *	\return	bool
//             *      Returns whether the widths match.
//             */
//			inline bool hasSameWidth(SynchrotronComponent& other) {
//				return this->getBitWidth() == other.getBitWidth();
//			}

			/**	\brief	Gets this SynchrotronComponent's state.
             *
             *	\return	std::bitset<bit_width>
             *      Returns the internal bitset.
             */
			inline std::bitset<bit_width> getState() const {
				return this->state;
			}

			/**	\brief	Gets the SynchrotronComponent's input connections.
             *
             *	\return	std::set<SynchrotronComponent*>&
             *      Returns a reference set to this SynchrotronComponent's inputs.
             */
			const std::set<SynchrotronComponent*>& getIputs() {
				return this->signalInput;
			}

			/**	\brief	Gets the SynchrotronComponent's output connections.
             *
             *	\return	std::set<SynchrotronComponent*>&
             *      Returns a reference set to this SynchrotronComponent's outputs.
             */
			const std::set<SynchrotronComponent*>& getOutputs() {
				return this->slotOutput;
			}

            /**	\brief	**[Thread safe]** Adds/Connects a new input to this SynchrotronComponent.
             *
             *	**Ensures both way connection will be made:**
             *	This will have input added to its inputs and input will have this added to its outputs.
             *
             *	\param	input
             *		The SynchrotronComponent to connect as input.
             */
			void addInput(SynchrotronComponent& input) {
				LockBlock lock(this);

				// deprecated? //if (!this->hasSameWidth(input)) return false;
				input.connectSlot(this);
			}

			/**	\brief	Adds/Connects a list of new inputs to this SynchrotronComponent.
             *
             *	Calls addInput() on each SynchrotronComponent* in inputList.
             *
             *	\param	inputList
             *		The list of SynchrotronComponents to connect as input.
             */
			void addInput(std::initializer_list<SynchrotronComponent*> inputList) {
				for(auto connection : inputList)
					this->addInput(*connection);
			}

            /**	\brief	**[Thread safe]** Removes/Disconnects an input to this SynchrotronComponent.
             *
             *	**Ensures both way connection will be removed:**
             *	This will have input removed from its inputs and input will have this removed from its outputs.
             *
             *	\param	input
             *		The SynchrotronComponent to disconnect as input.
             */
			void removeInput(SynchrotronComponent& input) {
				LockBlock lock(this);

				input.disconnectSlot(this);
			}

			/**	\brief	**[Thread safe]** Adds/Connects a new output to this SynchrotronComponent.
             *
             *	**Ensures both way connection will be made:**
             *	This will have output added to its outputs and output will have this added to its inputs.
             *
             *	\param	output
             *		The SynchrotronComponent to connect as output.
             */
			void addOutput(SynchrotronComponent& output) {
				LockBlock lock(this);

				// deprecated? //if (!this->hasSameWidth(*output)) return false;
				this->connectSlot(&output);
			}

			/**	\brief	Adds/Connects a list of new outputs to this SynchrotronComponent.
             *
             *	Calls addOutput() on each SynchrotronComponent* in outputList.
             *
             *	\param	outputList
             *		The list of SynchrotronComponents to connect as output.
             */
			void addOutput(std::initializer_list<SynchrotronComponent*> outputList) {
				for(auto connection : outputList)
					this->addOutput(*connection);
			}

			/**	\brief	**[Thread safe]** Removes/Disconnects an output to this SynchrotronComponent.
             *
             *	**Ensures both way connection will be removed:**
             *	This will have output removed from its output and output will have this removed from its inputs.
             *
             *	\param	output
             *		The SynchrotronComponent to disconnect as output.
             */
			void removeOutput(SynchrotronComponent& output) {
				LockBlock lock(this);

				this->disconnectSlot(&output);
			}

			/**	\brief	The tick() method will be called when one of this SynchrotronComponent's inputs issues an emit().
			 *
             *	\return	virtual void
             *		This method should be implemented by a derived class.
             */
			virtual inline void tick() {
				//LockBlock lock(this);
				std::bitset<bit_width> prevState = this->state;

				//std::cout << "Ticked\n";
				for(auto& connection : this->signalInput) {
					// Change this line to change the logic applied on the states:
					this->state |= ((SynchrotronComponent*) connection)->getState();
				}

				// Directly emit changes to subscribers on change
				if (prevState != this->state)
					this->emit();
			}

			/**	\brief	The emit() method will be called after a tick() completes to ensure the flow of new data.
			 *
			 *	Loops over all outputs and calls tick().
			 *
             *	\return	virtual void
             *		This method can be re-implemented by a derived class.
             */
			virtual inline void emit() {
				//LockBlock lock(this);

				for(auto& connection : this->slotOutput) {
					connection->tick();
				}
				//std::cout << "Emitted\n";
			}
	};

}


#endif // SYNCHROTRONCOMPONENT_HPP
