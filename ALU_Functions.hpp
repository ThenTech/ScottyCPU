// Alle staan in deze file, in dezelfde volgorde als in william zijn file. Nog niet alle functies zijn afgewerkt.

#ifndef SYNCHROTRONCOMPONENT_HPP
#define SYNCHROTRONCOMPONENT_HPP

#include <iostream>

#include <bitset>
#include <set>
#include <initializer_list>
#include <mutex>

namespace BaseFunctions {

    typedef struct{
        bool Carry_Out = false; // Carry
        bool A_Larger = false; // Jump if A is larger than B
        bool Equal = false; // Jump if A is equal to B
        bool Zero = false; // Jump if answer is Zero
    }Flags;

    class Instruction_SingleArg{
        public:
            template <size_t bit_width>
            virtual std::bitset<bit_width> execute(std::bitset<bit_width> &arg1 = 0;
    };

    class Instruction_SingleArg_With_Flag : public Instruction_SingleArg {
        private:
            Flags f;
        public:
            virtual Flags GetFlags() = 0;
    };

    class Instruction{
        public:
            template <size_t bit_width>
            virtual std::bitset<bit_width> execute(std::bitset<bit_width> &arg1, std::bitset<bit_width> &arg2) = 0;
    };

    class Instruction_With_Flag : public Instruction {
        private:
            Flags f;
        public:
            virtual Flags GetFlags() = 0;
    };


    class Add : public Instruction_With_Flag{
        public:

        template <size_t bit_width>

        std::bitset<bit_width> execute(const std::bitset<bit_width> &arg1, const std::bitset<bit_width> &arg2) {
            long long int t = arg1.to_ullong() + arg2.to_ullong();

            if (t > (2 << bit_width))
                this->f.Carry_Out = true;

            return std::bitset<bit_width>(t);
        }

    };

    class Subtract : public Instruction_With_Flag{
        public:

        template <size_t bit_width>

        std::bitset<bit_width> execute(const std::bitset<bit_width> &arg1, const std::bitset<bit_width> &arg2) {
            long long int t = arg1.to_ullong() - arg2.to_ullong();

            if (t > (2 << bit_width))
                this->f.Carry_Out = true;

            return std::bitset<bit_width>(t);
        }

    };

    class Multiply : public Instruction_With_Flag{
        public:

        template <size_t bit_width>

        std::bitset<bit_width> execute(const std::bitset<bit_width> &arg1, const std::bitset<bit_width> &arg2) {
            long long int t = arg1.to_ullong() * arg2.to_ullong();

            if (t > (2 << bit_width))
                this->f.Carry_Out = true;

            return std::bitset<bit_width>(t);
        }

    };

    class Divide : public Instruction_With_Flag{
        public:

        template <size_t bit_width>

        std::bitset<bit_width> execute(const std::bitset<bit_width> &arg1, const std::bitset<bit_width> &arg2) {
            long long int t = arg1.to_ullong() / arg2.to_ullong();

            if (t > (2 << bit_width))
                this->f.Carry_Out = true;

            return std::bitset<bit_width>(t);
        }

    };

    class SHL : public Instruction_With_Flag{
        public:

        template <size_t bit_width>

        std::bitset<bit_width> execute(const std::bitset<bit_width> &arg1, const size_t arg2 = 1) {
            long long int t = (arg1 << arg2).to_ullong();

            if (t > (2 << bit_width))
                this->f.Carry_Out = true;

            return std::bitset<bit_width>(t);
        }

    };

    class SHR : public Instruction{
        public:

        template <size_t bit_width>

        std::bitset<bit_width> execute(const std::bitset<bit_width> &arg1, const size_t arg2 = 1) {
            long long int t = (arg1 >> arg2).to_ullong();

            return std::bitset<bit_width>(t);
        }

    };

    class Compare : public Instruction{
        public:

        template <size_t bit_width>

        std::bitset<bit_width> execute(const std::bitset<bit_width> &arg1, const std::bitset<bit_width> arg2 = 1) {
            
            if arg1.to_ullong() == arg1.to_ullong(){
                this->f.Equal = true;
            };

            if arg1.to_ullong() > arg1.to_ullong(){
                this->f.A_Larger = true;
            };

            // arg1.to_ullong() < arg1.to_ullong() => f.A_Smaller => geheugenbesparing OP ALLE FUNCTIES indien weggelaten (wegens elke functie elke flag bezit).

            return 0;
        }

    };

    /* nog te doen:

        /// Logic Operators
        NOT, AND, NAND, OR, NOR, XOR,

        /// Memory Operators
        MOV, DATA,

    */

    class NOP : public Instruction{
        public:
            return 0;
    };

    /* nog te doen:

        /// Jump Instructions (without condition)
        JMP,

        /// Jump if [E]qual, [L]arger, [S]maller, [Z]ero, [C]arry
        JE, JL, JS, JZ,
        JLE, JSE,
        JCE, JCL, JCS, JCZ,
        JLZ, JSZ, JEZ,
        JCLE, JCLZ, JCEZ, JLEZ, JCLEZ,

        /// Clear Jump Flags
        CLF
    /*

}


#endif // SYNCHROTRONCOMPONENT_HPP



