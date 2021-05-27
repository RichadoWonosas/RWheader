#include "rw_bignum.h"

namespace stdrw
{

    BigNumber BigNumber::operator*(const BigNumber &another) const
    {
        class BigNumberTimesStatus
        {
        private:
            BigNumber temp_[7] = {BigNumber()};
            int status_ = STATUS_FIRST;
        public:
            enum
            {
                STATUS_FIRST,
                STATUS_SECOND,
                STATUS_LAST,
                STATUS_FIN
            };

            BigNumberTimesStatus() {}

            BigNumber &operator[](int i) { return temp_[(i < 0) ? 0 : (i % 7)]; }

            int Status() { return status_; }

            void Update(const BigNumber &result) { temp_[++status_ + 3] = result; }

            void Reset() { status_ = STATUS_FIRST; }
        };

        BigNumber a(*this), b(another), result;
        BigNumberTimesStatus stack[65] = {BigNumberTimesStatus()};
        i32 stack_top = 0;

        i32 i, length, temp_length[2];
        bool symbol = symbol_ != another.symbol_;
        u64 temp_num[4];

        a.symbol_ = b.symbol_ = SYMBOL_POSITIVE;

        while (stack_top >= 0)
        {
            switch (stack[stack_top].Status())
            {
                default:
                case BigNumberTimesStatus::STATUS_FIRST:
                    if ((a.length_ == 1) && (b.length_ == 1))
                    {
                        //Split the two number into 4 small blocks
                        temp_num[0] = (a.data_[0] >> 32) & 0xFFFFFFFF;
                        temp_num[1] = (a.data_[0]) & 0xFFFFFFFF;
                        temp_num[2] = (b.data_[0] >> 32) & 0xFFFFFFFF;
                        temp_num[3] = (b.data_[0]) & 0xFFFFFFFF;

                        //Get each block multified
                        stack[stack_top][0] = BigNumber(temp_num[0] * temp_num[2]);
                        stack[stack_top][1] = BigNumber(temp_num[0] * temp_num[3]);
                        stack[stack_top][2] = BigNumber(temp_num[1] * temp_num[2]);
                        stack[stack_top][3] = BigNumber(temp_num[1] * temp_num[3]);

                        //Get final result
                        result = (stack[stack_top][0] << 64) + ((stack[stack_top][1] + stack[stack_top][2]) << 32) +
                                 stack[stack_top][3];

                        if (--stack_top >= 0)
                            stack[stack_top].Update(result);
                    } else
                    {
                        //Get length that used for split
                        length = (a.length_ > b.length_) ? a.length_ : b.length_;
                        length = length - (length >> 1);

                        //Give all pointer a proper length
                        stack[stack_top][0].set_length((length < a.length_) ? length : a.length_);
                        stack[stack_top][1].set_length(
                                temp_length[0] = (a.length_ - length > 0) ? (a.length_ - length) : 0);
                        stack[stack_top][2].set_length((length < b.length_) ? length : b.length_);
                        stack[stack_top][3].set_length(
                                temp_length[1] = (b.length_ - length > 0) ? (b.length_ - length) : 0);

                        //Split the big numbers into smaller parts
                        for (i = 0; i < a.length_; i++)
                            if (i < stack[stack_top][0].length_)
                                stack[stack_top][0].data_[i] = a.data_[i];
                            else
                                stack[stack_top][1].data_[i - temp_length[0]] = a.data_[i];

                        for (i = 0; i < b.length_; i++)
                            if (i < stack[stack_top][2].length_)
                                stack[stack_top][2].data_[i] = b.data_[i];
                            else
                                stack[stack_top][3].data_[i - temp_length[1]] = b.data_[i];

                        //Calculate (t0 * t2)
                        a = stack[stack_top][0];
                        b = stack[stack_top][2];
                        ++stack_top;
                    }
                    break;

                case BigNumberTimesStatus::STATUS_SECOND:
                    //Calculate (t1 * t3)
                    a = stack[stack_top][1];
                    b = stack[stack_top][3];
                    ++stack_top;
                    break;

                case BigNumberTimesStatus::STATUS_LAST:
                    //Calculate ((t0 + t1) * (t2 + t3))
                    a = stack[stack_top][0] + stack[stack_top][1];
                    b = stack[stack_top][2] + stack[stack_top][3];
                    ++stack_top;
                    break;

                case BigNumberTimesStatus::STATUS_FIN:
                    //Calculate (t1 * t2 + t0 * t3)
                    stack[stack_top][6] -= stack[stack_top][4] + stack[stack_top][5];

                    //Shift each part to proper digit
                    stack[stack_top][5] <<= (static_cast<u64>(length) << 7);
                    stack[stack_top][6] <<= (static_cast<u64>(length) << 6);

                    //Just add each part together and get the answer!
                    result = stack[stack_top][4] + stack[stack_top][5] + stack[stack_top][6];

                    stack[stack_top].Reset();

                    if (--stack_top >= 0)
                        stack[stack_top].Update(result);
                    break;
            }
        }

        result.symbol_ = symbol;
        return result;
    }
}

//**** Legacy: old times method
//
//    BigNumber BigNumber::operator*(const BigNumber &another) const
//    {
//
//        BigNumber temp[7] = {BigNumber(0), BigNumber(0), BigNumber(0), BigNumber(0),
//                             BigNumber(0), BigNumber(0), BigNumber(0)};
//        i32 i, length, temp_length[2];
//        bool symbol = symbol_ != another.symbol_;
//        u64 temp_num[4];
//
//        // The base of iteration, for situation which both big number have only one block
//        if ((length_ == 1) && (another.length_ == 1))
//        {
//            //Split the two number into 4 small blocks
//            temp_num[0] = (data_[0] >> 32) & 0xFFFFFFFF;
//            temp_num[1] = (data_[0]) & 0xFFFFFFFF;
//            temp_num[2] = (another.data_[0] >> 32) & 0xFFFFFFFF;
//            temp_num[3] = (another.data_[0]) & 0xFFFFFFFF;
//
//            //Get each block multified
//            temp[0] = BigNumber(temp_num[0] * temp_num[2]);
//            temp[1] = BigNumber(temp_num[0] * temp_num[3]);
//            temp[2] = BigNumber(temp_num[1] * temp_num[2]);
//            temp[3] = BigNumber(temp_num[1] * temp_num[3]);
//
//            //Get final result
//            auto result = BigNumber((temp[0] << 64) + ((temp[1] + temp[2]) << 32) + temp[3]);
//
//            result.symbol_ = symbol;
//            return result;
//        } else
//        {
//            //Get length that used for split
//            length = (length_ > another.length_) ? length_ : another.length_;
//            length = length - (length >> 1);
//
//            //Give all pointer a proper length
//            temp[0].set_length((length < length_) ? length : length_);
//            temp[1].set_length(temp_length[0] = (length_ - length > 0) ? (length_ - length) : (0));
//            temp[2].set_length((length < another.length_) ? length : another.length_);
//            temp[3].set_length(temp_length[1] = (another.length_ - length > 0) ? (another.length_ - length) : 0);
//
//            //Split the big number
//            //Step 1: initialize the data_ array of higher part (temp1 and temp3)
//            if (temp_length[0] != 0)
//                temp[1].symbol_ = symbol_;
//            if (temp_length[1] != 0)
//                temp[3].symbol_ = another.symbol_;
//
//            //Step 2: Initialize the data_ array of lower part (temp0 and temp2)
//            temp[0].symbol_ = symbol_;
//            temp[2].symbol_ = another.symbol_;
//
//            //Step 3: Split the big numbers into smaller parts
//            for (i = 0; i < length_; i++)
//                if (i < temp[0].length_)
//                    temp[0].data_[i] = data_[i];
//                else
//                    temp[1].data_[i - temp_length[0]] = data_[i];
//
//            for (i = 0; i < another.length_; i++)
//                if (i < temp[2].length_)
//                    temp[2].data_[i] = another.data_[i];
//                else
//                    temp[3].data_[i - temp_length[1]] = another.data_[i];
//
//            //Iterate the multiply operation until small enough
//            //Cautious enough for allocated space release :)
//            //Step 1: Calculate (t0 * t2) and (t1 * t3)
//            temp[4] = temp[0] * temp[2];
//            temp[5] = temp[1] * temp[3];
//
//            //Step 2: Calculate (t1 * t2 + t0 * t3)
//            temp[6] = (temp[0] + temp[1]) * (temp[2] + temp[3]) - temp[4] - temp[5];
//
//            //Step 3: Shift each part to proper digit
//            temp[5] = temp[5] << (static_cast<u64>(length) << 7);
//            temp[6] = temp[6] << (static_cast<u64>(length) << 6);
//
//            //Step 5: Just add each part together and get the answer!
//            auto result = temp[4] + temp[5] + temp[6];
//            return result;
//        }
//    }
//}