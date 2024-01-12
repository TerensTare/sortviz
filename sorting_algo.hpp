
#pragma once

#include <algorithm>
#include <span>

#include "sorting_task.hpp"

namespace sortviz
{
    tnt::sorting_task bubble_sort(std::span<unsigned> numbers)
    {
        auto const n_1 = numbers.size() - 1;

        for (std::size_t i{}; i < n_1; ++i)
        {
            bool swapped = false;

            for (std::size_t j{}; j < n_1 - i; ++j)
            {
                co_yield {i, j};

                if (numbers[j] > numbers[j + 1])
                {
                    std::swap(numbers[j], numbers[j + 1]);
                    swapped = true;
                }
            }

            if (!swapped)
                break;
        }
    }

    tnt::sorting_task selection_sort(std::span<unsigned> numbers)
    {
        auto const n = numbers.size();
        auto const n_1 = n - 1;

        for (std::size_t i{}; i < n_1; ++i)
        {
            std::size_t min_idx = i;

            for (std::size_t j{i + 1}; j < n; ++j)
            {
                co_yield {i, j};

                if (numbers[j] < numbers[min_idx])
                    min_idx = j;
            }

            std::swap(numbers[i], numbers[min_idx]);
        }
    }

    tnt::sorting_task quick_sort(std::span<unsigned> numbers)
    {
        if (numbers.size() > 1)
        {
            auto const pivot = numbers.back();
            auto const pivot_it = std::partition(numbers.begin(), numbers.end() - 1,
                                                 [pivot](auto const &n)
                                                 { return n < pivot; });

            co_yield {std::size_t(pivot_it - numbers.begin()), numbers.size() - 1};
            std::swap(*pivot_it, numbers.back());

            auto const mid = pivot_it - numbers.begin();

            auto const left = numbers.subspan(0, mid);
            auto const right = numbers.subspan(mid + 1);

            for (auto [i, j] : quick_sort(left))
                co_yield {i, j};

            for (auto [i, j] : quick_sort(right))
                co_yield {i + mid + 1, j + mid + 1};
        }
    }

    tnt::sorting_task heapify(std::span<unsigned> numbers, std::size_t n, std::size_t i)
    {
        auto largest = i;
        auto const l = 2 * i + 1;
        auto const r = 2 * i + 2;

        if (l < n && numbers[l] > numbers[largest])
            largest = l;

        if (r < n && numbers[r] > numbers[largest])
            largest = r;

        if (largest != i)
        {
            co_yield {i, largest};
            std::swap(numbers[i], numbers[largest]);

            for (auto [i, j] : heapify(numbers, n, largest))
                co_yield {i, j};
        }
    }

    tnt::sorting_task heap_sort(std::span<unsigned> numbers)
    {
        auto const n = numbers.size();
        auto const pn = std::ptrdiff_t(n);

        for (std::ptrdiff_t i{pn / 2 - 1}; i >= 0; --i)
        {
            for (auto [i, j] : heapify(numbers, n, i))
                co_yield {i, j};
        }

        for (std::ptrdiff_t i{pn - 1}; i >= 0; --i)
        {
            co_yield {0, std::size_t(i)};

            std::swap(numbers[0], numbers[i]);

            for (auto [i, j] : heapify(numbers, i, 0))
                co_yield {i, j};
        }
    }
}