#ifndef CLOCK_HPP
#define CLOCK_HPP

#include <chrono>
#include <thread>
#include "SynchrotronComponentFixedInput.hpp"
#include "Exceptions.hpp"

using namespace std::chrono;

namespace CPUComponents {

	/** \brief
	 *		**Clock** : Generate a pulse on a certain frequency.
	 *
	 *
	 */
	template <size_t bit_width>
	class Clock : public SynchrotronComponentFixedInput<bit_width, 0u>{
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
				: SynchrotronComponentFixedInput<bit_width, 0u>(0) {
				this->setFrequency(frequency);
				this->reset();
			}

			/**
			 *	\brief	TO-DO
			 */
			~Clock() {}

			/**
			 *	\brief	TO-DO
			 */
			void setFrequency(float frequency) {
				if (!frequency || frequency < 0.0F)
					throw Exceptions::Exception("Clock frequency cannot be equal or smaller than zero!");
				this->freq = frequency;
				this->period = (long long int) (1e9 / frequency);
			}

			/**
			 *	\brief	TO-DO
			 */
			float getFrequency(void) const {
				return this->freq;
			}

			/**
			 *	\brief	TO-DO
			 */
			void setPeriod(float period) {
				if (!period || period < 0.0F)
					throw Exceptions::Exception("Clock period cannot be equal or smaller than zero!");
				this->freq = 1.0F / period;
				this->period = (long long int) (period * 1e9);
			}

			/**
			 *	\brief	TO-DO
			 */
			float getPeriod(void) const {
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
			bool startThread(void) {
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

			/**	\brief
			 *		The tick() method will be called when this Gate's input issues an emit().
			 */
			inline void tick() {
				this->state = 1;
				this->emit();
				this->state = 0;
				// Test load duration
				//std::this_thread::sleep_for(milliseconds(245));
			}
	};

}

#endif // CLOCK_HPP
