#include <iostream>
#include <math.h>
#include <stdlib.h>
#include "timer.h"
#include <time.h>

using namespace std;

void swap(int nums[], int i, int j);

double dshrandom(long input);

int quick_select(int nums[], int k, int low, int high);

int find_kth_smallest(int nums[], int n, int k);

int dSelect(int nums[], int k, int low, int high, int group_number);

int *generate_test_case(int n);

void quick_sort(int nums[], int low, int high);

int main() {

    std::cout << "project1 start!" << std::endl;
    // generate test case
    int n = 1000000;
    int *nums = generate_test_case(n);
    //find kth smallest
    cout << "test_case when n="<<n<<" has been generated!" << endl;
    for (int i = 0; i < n; i++) {
        cout << nums[i] << " ";
    }
    cout << endl;

    find_kth_smallest(nums, n, 6);

    delete (nums);
    return 0;
}

int *generate_test_case(int n) {
    int *nums = new int[n];
    int i = 0;
    while (i < n) {
        nums[i] = int(dshrandom(user_time()) * n);
//        nums[i] = rand()%100+0;
        i++;
    }
    return nums;
}

int find_kth_smallest(int nums[], int n, int k) {

    //NULL or 0, also k needs to be in [1 to n]
    if (nums == NULL || k > n || k <= 0) {
        cout << "not valid!" << endl;
        return -1;

    }
    //only one
    if (n == 1 && k == 1) {
        cout << "onlt one number!" << nums[0] << endl;
        return 0;
    }

    int *a = new int[n];
    int *b = new int[n];
    int *c = new int[n];
    int *d = new int[n];
    int *e = new int[n];
    for (int i = 0; i < n; i++) {
        a[i] = nums[i];
        b[i] = nums[i];
        c[i] = nums[i];
        d[i] = nums[i];
        e[i] = nums[i];
    }

    cout << "quickSort start!" << endl;
    start_timer();
    quick_sort(a, 0, n - 1);
    double quick_sort_time = elapsed_time();
    cout << "time for quickSort is: " << quick_sort_time<<endl;
    int quick_sort = a[k - 1];
    cout << "the result of quickSort then select is:" << k - 1 << ":" << quick_sort << endl;

    cout << "quickSelect start!" << endl;
    start_timer();
    int quickSelect = quick_select(b, k, 0, n - 1);
    double quick_select_time = elapsed_time();
    cout << "time for quickSelect is: " << quick_select_time<<endl;
    cout << "the result of quickSelect is:" << quickSelect << ":" << b[quickSelect] << endl;

    cout << "dSelect-5 start!" << endl;
    start_timer();
    int ds5 = dSelect(c, k, 0, n - 1, 5);
    double ds5_time = elapsed_time();
    cout << "time for dSelect-5 is: " << ds5_time<<endl;
    cout << "the result of dSelect-5 is:" << ds5 << ":" << c[ds5] << endl;

    cout << "dSelect-7 start!" << endl;
    start_timer();
    int ds7 = dSelect(d, k, 0, n - 1, 7);
    double ds7_time = elapsed_time();
    cout << "time for dSelect-7 is: " << ds7_time<<endl;
    cout << "the result of dSelect-7 is:" << ds7 << ":" << d[ds7] << endl;

    cout << "dSelect-3 start!" << endl;
    start_timer();
    int ds3 = dSelect(e, k, 0, n - 1, 3);
    double ds3_time = elapsed_time();
    cout << "time for dSelect-3 is: " << ds3_time<<endl;
    cout << "the result of dSelect-3 is:" << ds3 << ":" << e[ds3] << endl;


    delete (a);
    delete (b);
    delete (c);
    delete (d);
    delete (e);

    return 0;
}

void quick_sort(int nums[], int low, int high) {
    int min = (low + high) / 2;
    int i = low;
    int j = high;
    int pivot = nums[min];
    while (low < j || i < high) {
        while (nums[i] < pivot)
            i++;
        while (nums[j] > pivot)
            j--;
        if (i <= j) {
            swap(nums, i, j);
            i++;
            j--;
        } else {
            if (low < j)
                quick_sort(nums, low, j);
            if (i < high)
                quick_sort(nums, i, high);
            return;
        }
    }
}

int dSelect_partition(int nums[], int low, int high, int group_number) {
    int n = high - low + 1;
    // there are [group_count] groups and the rest at most 4 elements left
    int group_count = low - 1;

    for (int i = low; i + group_number - 1 <= high; i += group_number) {
        quick_sort(nums, i, i + group_number - 1);
        //swap medians to the left
        swap(nums, ++group_count, i + group_number / 2);
    }
    //find the median of medians use dselect
    int pivot = low + (group_count - low) / 2;
    dSelect(nums, pivot, low, group_count, group_number);
    //move el't lower then pivot to left, higher to right
    int pivot_value = nums[pivot];
    swap(nums, pivot, low);

    int i = low;
    int j = high;
    while (i < j) {
        while (nums[j] >= pivot_value && i < j) {
            j--;
        }
        nums[i] = nums[j];

        while (nums[i] <= pivot_value && i < j) {
            i++;
        }
        nums[j] = nums[i];
    }
    nums[i] = pivot_value;

    return i;
}

int dSelect(int nums[], int k, int low, int high, int group_number) {
    if (low >= high) {
        return low;
    }
    int n = high - low + 1;

    if (n <= 50) {
        quick_sort(nums, low, high);
        return low + k - 1;
    } else {
        int mid = dSelect_partition(nums, low, high, group_number);

        if ((mid - low) == k - 1) {
            return mid;
        }
        //count(L) > k, then kth in [low,mid-1]
        if ((mid - low) > k - 1) {
            return dSelect(nums, k, low, mid - 1, group_number);
        }//count(L) < k, then kth in [mid+1, high]
        else {
            return dSelect(nums, k - (mid - low + 1), mid + 1, high, group_number);
        }
    }
}

int random_partition(int nums[], int low, int high) {
    //int random_int = int(dshrandom(user_time()) * (high - low)) + low;
    int random_int = rand() % ((high - low)) + low;
    int pivot = nums[random_int];
    swap(nums, random_int, low);

    int i = low;
    int j = high;
    while (i < j) {
        while (nums[j] >= pivot && i < j) {
            j--;
        }
        nums[i] = nums[j];

        while (nums[i] <= pivot && i < j) {
            i++;
        }
        nums[j] = nums[i];
    }
    nums[i] = pivot;
    return i;
}

int quick_select(int nums[], int k, int low, int high) {
    //randomly select a pivot from 0 - (n-1)
    if (high == low) {
        return low;
    }
    int mid = random_partition(nums, low, high);
    //count(L) == k
    if ((mid - low) < k - 1) {
        return quick_select(nums, k - (mid - low + 1), mid + 1, high);
    }
    //count(L) > k, then kth in [low,mid-1]
    if ((mid - low) > k - 1) {
        return quick_select(nums, k, low, mid - 1);
    }//count(L) < k, then kth in [mid+1, high]
    else {
        return mid;
    }
}


void swap(int nums[], int i, int j) {
    int temp = nums[i];
    nums[i] = nums[j];
    nums[j] = temp;
}

double dshrandom(long input) {
/*  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  */
/*	Random Number Generator					*/
/*		first called with seed				*/
/*		thereafter called with 0			*/
/*	Based on code appearing in				*/
/*	"Random number generators: good ones are hard to find"	*/
/*		by Stephen Park and Keith Miller		*/
/*		CACM 31 (Oct 1988) 1192-1201.			*/
/*  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  */
    static long seed = 3125;
    static long a = 16807;
    static long m = 2147483647;
    static long q = 127773;      /* m div a */
    static long r = 2836;        /* m mod a */
    long lo, hi, test;

    if (input > 0) seed = input;
    hi = seed / q;
    lo = seed - (hi * q);
    test = a * lo - r * hi;
    if (test > 0) seed = test;
    else seed = test + m;
    /* seed equally in range 0...2^31 -1  */
    return (((double) seed) / (double) m);
}