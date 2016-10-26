#ifndef CLOCK_HPP
#define CLOCK_HPP

#include <chrono>
#include <thread>
#include "SynchrotronComponentFixedInput.hpp"

using namespace std::chrono;

namespace CPUComponents {

	/**
	 *	\brief	TO-DO
	 */
	class Clock : public SynchrotronComponentFixedInput<1u, 1u>{
		private:
			/**
			 *	\brief	TO-DO
			 */
			time_point<high_resolution_clock> startTime;

			/**
			 *	\brief	TO-DO
			 */
			float freq;

			/**
			 *	\brief	in nanoseconds
			 */
			long long int period;
		public:
			/**
			 *	Default constructor
			 *
			 *	\param	frequency
			 *			Frequency in Hz.
			 */
			Clock(float frequency)
				: SynchrotronComponentFixedInput<1u, 1u>(0) {
				this->setFrequency(frequency);
				this->reset();
			}

			/**	Copy constructor
			 *	\param	Other
			 *		SynchrotronComponent to copy from
			 *	\param	duplicateAll_IO
			 *		Specifies whether to only copy inputs (false) or outputs as well (true).
			 */
			Clock(const SynchrotronComponent<1u>& other, bool duplicateAll_IO = false)
				: SynchrotronComponentFixedInput<1u, 1u>(other, duplicateAll_IO) {}

			/**	\brief
			 *	Connection constructor
			 *	*	Adds signal subscriptions from inputList
			 *	*	Optionally adds slot subscribers from outputList
			 *
			 *	\param	inputList
			 *		The list of SynchrotronComponents to connect as input.
			 *	\param	outputList
			 *		The list of SynchrotronComponents to connect as output.
			 */
			Clock(std::initializer_list<SynchrotronComponent<1u>*> inputList,
					std::initializer_list<SynchrotronComponent<1u>*> outputList = {} )
									: SynchrotronComponentFixedInput<1u, 1u>(inputList, outputList) {}

			/**
			 *	\brief	TO-DO
			 */
			~Clock() {}

			/**
			 *	\brief	TO-DO
			 */
			void setFrequency(float frequency) {
				this->freq = frequency;
				this->period = (long long int) (1e9 / frequency);
			}

			/**
			 *	\brief	TO-DO
			 */
			float getFrequency(void) {
				return this->freq;
			}

			/**
			 *	\brief	TO-DO
			 */
			void setPeriod(float period) {
				this->freq = 1.0F / period;
				this->period = (long long int) (period * 1e9);
			}

			/**
			 *	\brief	TO-DO
			 */
			float getPeriod(void) {
				return this->period / 1e9F;
			}

			/**
			 *	\brief	TO-DO
			 */
			void reset(void) {
				this->startTime = high_resolution_clock::now();
			}

			/**
			 *	\brief	TO-DO
			 */
			bool startThread() {
				std::thread clk(*this);
				clk.detach();
				return !clk.joinable();
			}

			/**
			 *	\brief	TO-DO
			 */
			void operator()(void) {
				long long int difference = 0;

				while(true) {
					std::this_thread::sleep_until(this->startTime + nanoseconds(this->period));

					/**********************************************************************/
//					printf("Elapsed: %.7f s  ::  waited for: %.7f s\n",
//								(duration_cast<nanoseconds>(
//									high_resolution_clock::now() - this->startTime).count()
//								/ 1e9F),
//								(this->period - difference) / 1e9F);
					/**********************************************************************/

					this->reset();
					this->tick();

					#ifdef THROW_EXCEPTIONS
						difference = duration_cast<nanoseconds>(high_resolution_clock::now() - this->startTime).count();

						if (difference > this->period)
							printf("[WARNING] Clock period (%.03fs) is smaller than execution time (%.3fs)!\n", this->period / 1e9F, difference / 1e9F);
					#endif
				}
			}

			/**	\brief
			 *		The tick() method will be called when this Gate's input issues an emit().
			 */
			inline void tick() {
//				#ifdef THROW_EXCEPTIONS
//					if (this->getInputs().size() != 0)
//						throw Exceptions::Exception("[ERROR] Clock cannot have inputs!");
//				#endif

				this->state = 1;
				this->emit();
				this->state = 0;
				// Test load duration
				//std::this_thread::sleep_for(milliseconds(245));
			}
	};

}

#endif // CLOCK_HPP
