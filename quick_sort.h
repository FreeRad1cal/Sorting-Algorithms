#pragma once

#include <utility>
#include <iterator>
#include <cassert>
#include <stdexcept>
#include "merge_sort.h"

namespace BK_sort
{

	template<typename Iterator, typename Compare = std::less<typename std::iterator_traits<Iterator>::value_type>>
	void
	quicksort(Iterator begin, Iterator end, Compare comp = Compare())
	{
		//algorithm entry point
		_quicksort(begin, end, comp, typename std::iterator_traits<Iterator>::iterator_category());
	}

	template<typename RandomAccessIterator, typename Compare>
	void
	_quicksort(RandomAccessIterator begin, RandomAccessIterator end, Compare &comp, const std::random_access_iterator_tag &tag)
	{
		using iterator_category = typename std::iterator_traits<RandomAccessIterator>::iterator_category;

		if (end - begin > 20)
		{
			RandomAccessIterator p = partition(begin, end, comp, iterator_category());
			//[begin,p]
			_quicksort(begin, p + 1, comp, tag);
			//(p,end)
			_quicksort(p + 1, end, comp, tag);
		}
		else
			_insertion_sort(begin, end, comp, tag);
	}

	template<typename ForwardIterator, typename Compare>
	void
	_quicksort(ForwardIterator begin, ForwardIterator end, Compare &comp, const std::forward_iterator_tag &tag)
	{
		using iterator_category = typename std::iterator_traits<ForwardIterator>::iterator_category;

		if (begin != end && std::next(begin) != end)
		{
			ForwardIterator p = partition(begin, end, comp, iterator_category());
			//[begin,p)
			_quicksort(begin, p, comp, tag);
			//[p,end)
			_quicksort(std::next(p), end, comp, tag);
		}
	}

	template<typename RandomAccessIterator, typename Compare>
	RandomAccessIterator
	median_of_three(RandomAccessIterator begin, RandomAccessIterator end, Compare &comp)
	{
		using value_type = typename std::iterator_traits<RandomAccessIterator>::value_type;

		if (end - begin < 3)
			throw std::invalid_argument("median_of_three requires at least 3 values");

		RandomAccessIterator temp[3];
		temp[0] = begin;
		temp[1] = midpoint(begin, end, std::random_access_iterator_tag());
		temp[2] = (end - 1);

		insertion_sort(temp, temp + 2,
			[&comp](RandomAccessIterator a, RandomAccessIterator b) ->bool
		{
			return comp(*a, *b);
		}
		);

		return temp[1];
	}

	template<typename RandomAccessIterator, typename Compare>
	RandomAccessIterator
	partition(RandomAccessIterator begin, RandomAccessIterator end, Compare &comp, const std::random_access_iterator_tag &tag)
	{
		using std::swap;
		using value_type = typename std::iterator_traits<RandomAccessIterator>::value_type;

		assert(end - begin > 1);

		//find a pivot 
		value_type pivot = *begin;
		try
		{
			//median_of_three requires random access iterators
			pivot = *median_of_three(begin, end, comp);
		}
		catch (...)
		{
			//pivot = *begin
		}

		//move two indexes toward each other until e > b
		for (auto b = begin, e = end - 1; true; swap(*b, *e), ++b, --e)
		{
			while (comp(*b, pivot))
				++b;

			while (comp(pivot, *e))
				--e;

			//the == condition is possible if a value in the range is equal to the pivot
			if (b >= e)
				return e;
		}
	}

	template<typename BidirectionalIterator, typename Compare>
	BidirectionalIterator
	partition(BidirectionalIterator begin, BidirectionalIterator end, Compare &comp, const std::bidirectional_iterator_tag &tag)
	{
		using std::swap;

		if (begin == end)
			return begin; //do nothing

						  //find a pivot 
		BidirectionalIterator pivot = std::prev(end), swap_pos = begin;

		//stable sort
		for (auto it = begin, end_it = pivot; it != end_it; ++it)
		{
			if (comp(*it, *pivot))
			{
				if (swap_pos != it)
					swap(*it, *swap_pos);
				++swap_pos;
			}
		}

		swap(*pivot, *swap_pos);
		return swap_pos;
	}

	template<typename ForwardIterator, typename Compare>
	ForwardIterator
	partition(ForwardIterator begin, ForwardIterator end, Compare &comp, const std::forward_iterator_tag &tag)
	{
		using std::swap;

		if (begin == end)
			return begin; //do nothing

						  //choose a pivot 
		ForwardIterator pivot = begin;

		//boundary is the last element that is smaller than  the pivot
		auto boundary = pivot, precursor = pivot;
		//noswap is true if no element smaller than the pivot is found (array already sorted)
		//first is true if no larger values have been found so far
		bool noswap = true, first = true;
		for (auto cursor = std::next(boundary); cursor != end; ++cursor, ++precursor)
		{
			if (comp(*cursor, *pivot)) //if the element is smaller than the pivot
			{
				auto next_boundary = std::next(boundary);
				//swap with the last known larger element
				if (next_boundary != cursor) //don't self-swap
					swap(*next_boundary, *cursor);
				//advance the index of the last known larger element
				++boundary;
			}
			else
			{
				//the first instance of an element that is larger than the pivot
				if (first)
				{
					boundary = precursor;
					first = false;
				}
				continue;
			}

			//if the control reaches this point, then an element smaller than the pivot has been found
			noswap = false;
		}

		//return the first element that is >= to the pivot (the pivot itself)
		if (noswap)
		{
			return pivot;
		}
		else
		{
			swap(*boundary, *pivot);
			return boundary;
		}
	}
}