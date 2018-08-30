#pragma once

#include <iterator>
#include <utility>

namespace BK_sort
{
	template<typename ForwardIterator, typename Compare = std::less<typename std::iterator_traits<ForwardIterator>::value_type>>
	void
		merge_sort(ForwardIterator begin, ForwardIterator end, Compare comp = Compare())
	{
		//alogrithm entry point
		_merge_sort(begin, end, comp, typename std::iterator_traits<ForwardIterator>::iterator_category());
	}

	template<typename RandomAccessIterator>
	RandomAccessIterator
		midpoint(RandomAccessIterator begin, RandomAccessIterator end, const std::random_access_iterator_tag &tag)
	{
		return begin + ((end - begin) >> 1);
	}

	template<typename ForwardIterator>
	ForwardIterator
		midpoint(ForwardIterator begin, ForwardIterator end, const std::input_iterator_tag &tag)
	{
		typename std::iterator_traits<ForwardIterator>::difference_type mid_count = 0;
		//find the difference between begin and end
		for (auto it = begin; it != end; ++it, ++mid_count);
		//generate the midpoint iterator
		for (auto c = mid_count >> 1; c > 0; --c)
			++begin;
		return begin;
	}

	template<typename RandomAccessIterator, typename Compare>
	void
		_merge_sort(RandomAccessIterator begin, RandomAccessIterator end, Compare &comp, const std::random_access_iterator_tag &tag)
	{
		using iterator_category = typename std::iterator_traits<RandomAccessIterator>::iterator_category;
		using value_type = typename std::iterator_traits<RandomAccessIterator>::value_type;

		if (end - begin > 10)
		{
			RandomAccessIterator mid = midpoint(begin, end, iterator_category());

			_merge_sort(begin, mid, comp, iterator_category());
			_merge_sort(mid, end, comp, iterator_category());

			std::vector<value_type> buffer;

			auto a = begin, b = mid;
			while (a != mid && b != end)
			{
				//stable merge
				if (comp(*b, *a))
				{
					buffer.emplace_back(std::move(*b));
					++b;
				}
				else
				{
					buffer.emplace_back(std::move(*a));
					++a;
				}
			}

			if (a != mid)
			{
				while (a != mid)
				{
					buffer.emplace_back(std::move(*a));
					++a;
				}
			}

			for (auto it = buffer.begin(), end_it = buffer.end(); it != end_it; ++it, ++begin)
				*begin = std::move(*it);
		}
		else
			_insertion_sort(begin, end, comp, tag);
	}

	template<typename ForwardIterator, typename Compare>
	void
		_merge_sort(ForwardIterator begin, ForwardIterator end, Compare &comp, const std::forward_iterator_tag &tag)
	{
		using iterator_category = typename std::iterator_traits<ForwardIterator>::iterator_category;
		using value_type = typename std::iterator_traits<ForwardIterator>::value_type;

		if (std::next(begin) != end)
		{
			ForwardIterator mid = midpoint(begin, end, iterator_category());

			_merge_sort(begin, mid, comp, iterator_category());
			_merge_sort(mid, end, comp, iterator_category());

			std::vector<value_type> buffer;

			auto a = begin, b = mid;
			while (a != mid && b != end)
			{
				//stable merge
				if (comp(*b, *a))
				{
					buffer.emplace_back(std::move(*b));
					++b;
				}
				else
				{
					buffer.emplace_back(std::move(*a));
					++a;
				}
			}

			if (a != mid)
			{
				while (a != mid)
				{
					buffer.emplace_back(std::move(*a));
					++a;
				}
			}

			for (auto it = buffer.begin(), end_it = buffer.end(); it != end_it; ++it, ++begin)
				*begin = std::move(*it);
		}
	}
}