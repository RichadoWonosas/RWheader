#pragma once

#ifndef __RW_MSORT__
#define __RW_MSORT__ 100

namespace stdrw
{
    enum EnumMergeSortParameters
    {
        ORDER_SMALL2BIG = 0,
        ORDER_BIG2SMALL,
        STATUS_LEFT_SORT = 0,
        STATUS_RIGHT_SORT,
        STATUS_MERGING
    };

    class MergeParams final
    {
    private:
        long start_position_;
        long length_;
        long status_;

    public:
        explicit MergeParams(long start = 0, long len = 1) : start_position_(start), length_(len),
                                                             status_(STATUS_LEFT_SORT) {}

        long get_start_position() const { return start_position_; }

        long get_length() const { return length_; }

        long get_current_status() const { return status_; }

        bool ShiftToNextStatus()
        {
            if (status_ == STATUS_MERGING)
                return false;
            status_++;
            return true;
        }
    };

    template<typename datatype>
    class MergeSort final
    {
    public:
        static void SortByDefaultCompareOperator(datatype *data, long amount, int order);

        static void SortByGivenCompareFunction(datatype *data, long amount,
                                               bool (*ShouldBeInFrontOf)(const datatype &a, const datatype &b));
    };

} // namespace stdrw

template<typename datatype>
void stdrw::MergeSort<datatype>::SortByDefaultCompareOperator(datatype *data, long amount, int order)
{
    auto *stack = new MergeParams[amount >> 1];
    long stack_top = 0;

    long i, j, k, sta, len;
    MergeParams *param;
    datatype temp;
    auto *res = new datatype[amount];

    stack[0] = MergeParams(0, amount);

    while (stack_top != -1)
    {
        param = &stack[stack_top];

        sta = param->get_start_position();
        len = param->get_length();

        switch (len)
        {
            case 0:
            case 1:
                while (param->ShiftToNextStatus());
                break;

            case 2:
                if ((order == ORDER_SMALL2BIG && data[sta] > data[sta + 1]) ||
                    (order == ORDER_BIG2SMALL && data[sta] < data[sta + 1]))
                {
                    temp = data[sta];
                    data[sta] = data[sta + 1];
                    data[sta + 1] = temp;
                }
                while (param->ShiftToNextStatus());
                break;

            default:
                switch (param->get_current_status())
                {
                    default:
                    case STATUS_LEFT_SORT:
                        stack_top++;
                        stack[stack_top] = MergeParams(sta, len >> 1);
                        break;

                    case STATUS_RIGHT_SORT:
                        stack_top++;
                        stack[stack_top] = MergeParams(sta + (len >> 1), len - (len >> 1));
                        break;

                    case STATUS_MERGING:
                        for (k = i = 0, j = (len >> 1); (i < (len >> 1)) && (j < len);)
                            res[k++] = (((order == ORDER_SMALL2BIG) && (data[sta + i] > data[sta + j])) ||
                                        ((order == ORDER_BIG2SMALL) && (data[sta + i] < data[sta + j])))
                                       ? data[sta + (j++)]
                                       : data[sta + (i++)];

                        while (i < (len >> 1))
                            res[k++] = data[sta + (i++)];

                        while (j < len)
                            res[k++] = data[sta + (j++)];

                        for (i = 0; i < len; i++)
                            data[sta + i] = res[i];

                        break;
                }
                break;
        }
        if (!param->ShiftToNextStatus())
            stack_top--;
    }

    delete[] stack;
    delete[] res;
}

template<typename datatype>
void stdrw::MergeSort<datatype>::SortByGivenCompareFunction(datatype *data, long amount,
                                                            bool (*ShouldBeInFrontOf)(const datatype &a,
                                                                                      const datatype &b))
{
    auto *stack = new MergeParams[amount >> 1];
    long stack_top = 0;

    long i, j, k, sta, len;
    MergeParams *param;
    datatype temp;
    auto *res = new datatype[amount];

    stack[0] = MergeParams(0, amount);

    while (stack_top != -1)
    {
        param = &stack[stack_top];

        sta = param->get_start_position();
        len = param->get_length();

        switch (len)
        {
            case 0:
            case 1:
                while (param->ShiftToNextStatus());
                break;

            case 2:
                if (ShouldBeInFrontOf(data[sta + 1], data[sta]))
                {
                    temp = data[sta];
                    data[sta] = data[sta + 1];
                    data[sta + 1] = temp;
                }
                while (param->ShiftToNextStatus());
                break;

            default:
                switch (param->get_current_status())
                {
                    default:
                    case STATUS_LEFT_SORT:
                        stack_top++;
                        stack[stack_top] = MergeParams(sta, len >> 1);
                        break;

                    case STATUS_RIGHT_SORT:
                        stack_top++;
                        stack[stack_top] = MergeParams(sta + (len >> 1), len - (len >> 1));
                        break;

                    case STATUS_MERGING:
                        for (k = i = 0, j = (len >> 1); (i < (len >> 1)) && (j < len);)
                            res[k++] = (ShouldBeInFrontOf(data[sta + j], data[sta + i]))
                                       ? data[sta + (j++)]
                                       : data[sta + (i++)];

                        while (i < (len >> 1))
                            res[k++] = data[sta + (i++)];

                        while (j < len)
                            res[k++] = data[sta + (j++)];

                        for (i = 0; i < len; i++)
                            data[sta + i] = res[i];

                        break;
                }
                break;
        }
        if (!param->ShiftToNextStatus())
            stack_top--;
    }

    delete[] stack;
    delete[] res;
}

#endif // __RW_MSORT__