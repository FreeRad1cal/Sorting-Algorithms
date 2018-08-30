#pragma once

#include <utility>
#include <iterator>

namespace BK_sort
{

	template<typename ForwardIterator>
	void selection_sort(ForwardIterator begin, ForwardIterator end)
	{
		using std::swap;

		for (auto it = begin, smallest = std::next(begin); it != end; ++begin, it = std::next(begin))
		{
			while (it != end)
			{
				if (*it < *smallest)
					smallest = it;
				++it;
			}

			swap(*begin, *smallest);
		}
	}

	template<typename BidirectionalIterator, typename Compare = std::less<typename std::iterator_traits<BidirectionalIterator>::value_type>>
	void insertion_sort(BidirectionalIterator begin, BidirectionalIterator end, Compare comp = Compare())
	{
		_insertion_sort(begin, end, comp, typename std::iterator_traits<BidirectionalIterator>::iterator_category());
	}

	template<typename BidirectionalIterator, typename Compare>
	void _insertion_sort(BidirectionalIterator begin, BidirectionalIterator end, Compare &comp, const std::bidirectional_iterator_tag &ignore)
	{
		using std::swap;

		auto it = begin;
		for (++it; it != end; ++it)
		{
			for (auto j = it, k = j; j != begin; --j)
			{
				--k;
				if (comp(*j, *k))
					swap(*j, *k);
				else
					break;
			}
		}
	}

	template<typename RandomAccessIterator, typename Compare>
	void _insertion_sort(RandomAccessIterator begin, RandomAccessIterator end, Compare &comp, const std::random_access_iterator_tag &ignore)
	{
		using std::swap;

		for (auto i = begin + 1; i != end; ++i)
		{
			for (auto j = i, k = i; j != begin; --j)
			{
				--k;
				if (comp(*j, *k))
					swap(*j, *k);
				else
					break;
			}
		}
	}
}