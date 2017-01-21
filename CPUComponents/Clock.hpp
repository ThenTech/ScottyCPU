#ifndef CLOCK_HPP
#define CLOCK_HPP

#include <chrono>
#include <thread>
#include "../SynchrotronComponentFixedInput.hpp"
#include "../Exceptions.hpp"

using namespace std::chrono;

namespace CPUComponents {

	/** \brief	**Clock** : Generate a pulse on a certain frequency.
	 *
	 *	\tparam	bit_width
	 *		This template argument specifies the width of the in and output connections.
	 */
	template <size_t bit_width>
	class Clock : public SynchrotronComponentFixedInput<bit_width, 0u>{
		private:
			/**
			 *	\brief	The point in time where the current pulse started.
			 */
			time_point<high_resolution_clock> startTime;

			/**
			 *	\brief	The clock frequency in Hz.
			 */
			float freq;

			/**
			 *	\brief	The clock period in nanoseconds.
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
				: SynchrotronComponentFixedInput<bit_width, 0u>(0) {
				this->setFrequency(frequency);
				this->reset();
			}

			/**
			 *	\brief	Default destructor
			 */
			~Clock() {}

			/**
			 *	\brief	Set the Clock frequency.
			 *
			 *			Also sets period = (1 / frequency).
			 *
			 *	\param	frequency
			 *			The frequency to set. Must be greater than 0.0.
			 *	\exception	Exceptions::Exception
			 *		Throws exception if frequency is less than or equal to zero (undefined behaviour).
			 */
			void setFrequency(float frequency) {
				if (!frequency || frequency < 0.0F)
					throw Exceptions::Exception("Clock frequency cannot be equal or smaller than zero!");
				this->freq = frequency;
				this->period = (long long int) (1e9 / frequency);
			}

			/**
			 *	\brief	Return the clock frequency in Hz.
			 */
			float getFrequency(void) const {
				return this->freq;
			}

			/**
			 *	\brief	Set the Clock period.
			 *
			 *			Also sets frequency = (1 / period).
			 *
			 *	\param	period
			 *			The period to set. Must be greater than 0.0.
			 *	\exception	Exceptions::Exception
			 *		Throws exception if period is less than or equal to zero (undefined behaviour).
			 */
			void setPeriod(float period) {
				if (!period || period < 0.0F)
					throw Exceptions::Exception("Clock period cannot be equal or smaller than zero!");
				this->freq = 1.0F / period;
				this->period = (long long int) (period * 1e9);
			}

			/**
			 *	\brief	Return the clock period in seconds.
			 */
			float getPeriod(void) const {
				return this->period / 1e9F;
			}

			/**
			 *	\brief	Reset the clock by setting the startTime to now().
			 */
			inline void reset(void) {
				this->startTime = high_resolution_clock::now();
			}

			/**
			 *	\brief	Start a detached thread to run the clock.
			 *
			 *	\return	bool
			 *		Returns true if thread has been detached.
			 */
			bool startThread(void) {
				std::thread clk(*this);
				clk.detach();
				return !clk.joinable();
			}

			/**
			 *	\brief	The function called when the clock is started.
			 *
			 *			Uses sleep::untill on this this thread to wait for the setted period.
			 */
			void operator()(void) {
				long long int difference = 0;

				while(true) {
					std::this_thread::sleep_until(this->startTime + nanoseconds(this->period));

					/**********************************************************************/
					printf("Elapsed: %.7f s  ::  waited for: %.7f s\n",
								(duration_cast<nanoseconds>(
									high_resolution_clock::now() - this->startTime).count()
								/ 1e9F),
								(this->period - difference) / 1e9F);
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

			/**	\brief	The tick() method will be called when this Gate's input issues an emit().
			 */
			inline void tick(void) {
				this->state = 1;
				this->emit();
				this->state = 0;
				// Test load duration
				//std::this_thread::sleep_for(milliseconds(245));
			}
	};

}

#endif // CLOCK_HPP
