##################################################################################
# This file is a part of Assignment 2 for the course Algorithms & Analysis at RMIT
# Author: Son Hoang Dau, RMIT 2022
##################################################################################
import copy
import math
import time


def parent_of(i):
    """
    return the parent index of i
    @param i: the current index
    @return: the parent index of i
    """
    if i <= 0:
        return None
    return math.floor((i - 1) / 2)


def right_child_of(i, n):
    """
    returns the index of the right child of i
    @param i: current node (index)
    @param n: number of tree nodes
    @return: the index of the right child of i, None if i has no right child
    """
    right_child = 2 * i + 2
    if right_child < n:
        return right_child
    else:
        return None


def left_child_of(i, n):
    """
    returns the index of the left child of i
    @param i: current node (index)
    @param n: number of tree nodes
    @return: the index of the left child of i, None if i has no left child
    """
    left_child = 2 * i + 1
    if left_child < n:
        return left_child
    else:
        return None


def build_magical_tree(n):
    ret = None
    # p is the container to contain permutations
    p = [None] * n
    # nums contains available numbers to choose from
    nums = set(range(0, n))
    # mls contains available magical labels
    mls = set(range(1, n))
    if n <= 10:
        ret = build_magical_tree_recursive(p, 0, nums, mls, False)
        print(" ".join([str(num) for num in ret]))
    else:  # Set up the most optimal starting permutation position for n > 10
        p[0] = 1 + n % 2  # either 1 or 0
        p[1] = 0
        p[2] = n - 3
        p[3] = n - 1
        p[4] = n - 2

        nums.remove(1 + n % 2)
        nums.remove(0)
        nums.remove(n - 3)
        nums.remove(n - 1)
        nums.remove(n - 2)

        mls.remove(1 + n % 2)
        mls.remove(n - 3 - (1 + n % 2))
        mls.remove(n - 1)
        mls.remove(n - 2)

        # Pass the leftmost index whose value is None
        ret = build_magical_tree_recursive(p, 5, nums, mls, False)
        print(" ".join([str(num) for num in ret]))


def build_magical_tree_recursive(p, idx, nums, mls, is_ml_found):
    ret = None
    for num in nums:
        if not is_ml_found:
            # If the insertion is valid according to the magical labelling(ml) property, a copy of mls is returned
            # otherwise None is returned.
            mls_copy = update_mls(p, idx, num, mls)
            if mls_copy is not None:
                p_copy = copy.deepcopy(p)
                nums_copy = copy.deepcopy(nums)
                p_copy[idx] = num
                nums_copy.remove(num)
                # Once the number is selected for the current idx, try the next idx whose value is None recursively.
                if None in p_copy:
                    left_most_none_idx = p_copy.index(None)
                    ret = build_magical_tree_recursive(
                        p_copy, left_most_none_idx, nums_copy, mls_copy, is_ml_found
                    )
                    # If ret is the same length as the required number of numbers
                    if ret is not None and len(ret) == len(p):
                        is_ml_found = True
            # If there is no more mls to choose from
            if mls_copy is not None and len(mls_copy) == 0:
                is_ml_found = True
                ret = p_copy
    return ret


def update_mls(p, idx, num, mls):
    # Return true if mls is successfully updated or false if ml is already in use
    diffs = []
    is_unique = True
    mls_copy = None

    # Check whether the selected number results in a unique label in relation to parent_idx
    parent_idx = parent_of(idx)
    # If parent exists
    if parent_idx is not None:
        diff = abs(p[parent_idx] - num)
        if diff not in mls:
            is_unique = False
        else:
            diffs.append(diff)

    # Check whether the selected number results in a unique label in relation to left_child_idx
    left_child_idx = left_child_of(idx, len(p))
    if is_unique and left_child_idx and p[left_child_idx] is not None:
        diff = abs(p[left_child_idx] - num)
        if diff not in mls:
            is_unique = False
        else:
            diffs.append(diff)

    # Check whether the selected number results in a unique label in relation to right_child_idx
    right_child_idx = right_child_of(idx, len(p))
    if is_unique and right_child_idx and p[right_child_idx] is not None:
        diff = abs(p[right_child_idx] - num)
        if diff not in mls:
            is_unique = False
        else:
            diffs.append(diff)

    # Remove all the selected mls from above
    if is_unique:
        mls_copy = copy.deepcopy(mls)
        for diff in diffs:
            mls_copy.remove(diff)

    return mls_copy


def main():
    start_time = time.time()
    for n in range(1, 20):
        build_magical_tree(n)
    print(time.time() - start_time)


if __name__ == "__main__":
    main()
