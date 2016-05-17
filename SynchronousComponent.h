#ifndef SYNCHRONOUSCOMPONENT_H
#define SYNCHRONOUSCOMPONENT_H

namespace CPUComponents {

#define ARCHITECTURE	16

	class SynchronousComponent {
		public:
			SynchronousComponent() {}

			virtual void tick() {}
	};

}

#endif // SYNCHRONOUSCOMPONENT_H
