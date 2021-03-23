#include <cassert>
#include "whatever.hpp"

void array_test()
{
    {
        const int const_arr[] = {1, 2, 3};
        utils::whatever any(const_arr);
        assert(utils::whatever_cast<const int *>(any) == const_arr);
        assert(nullptr == utils::whatever_cast<int *>(&any));

        int arr[] = {1, 2, 3};
        any = arr;
        assert(utils::whatever_cast<int *>(any) == arr);
        assert(nullptr == utils::whatever_cast<const int *>(&any));
    }

    {
        int arr[] = {1, 2, 3};
        utils::whatever any(arr);
        assert(utils::whatever_cast<int *>(any) == arr);
        assert(nullptr == utils::whatever_cast<const int *>(&any));

        const int const_arr[] = {1, 2, 3};
        any = const_arr;
        assert(utils::whatever_cast<const int *>(any) == const_arr);
        assert(nullptr == utils::whatever_cast<int *>(&any));
    }
}

int main()
{
    array_test();
    return 0;
}
