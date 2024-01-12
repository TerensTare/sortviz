
#pragma once

#include <coroutine>
#include <iterator>

namespace tnt
{
    struct colors final
    {
        std::size_t red = 0;
        std::size_t blue = 0;
    };

    namespace detail
    {
        template <typename T>
        struct generator_iterator final
        {
            constexpr generator_iterator &operator++()
            {
                coro->resume();
                return *this;
            }

            constexpr auto &operator*() { return coro->value(); }
            constexpr auto const &operator*() const { return coro->value(); }

            constexpr explicit operator bool() const noexcept
            {
                return !coro->done();
            }

            constexpr bool operator==(std::default_sentinel_t) const noexcept
            {
                return coro->done();
            }

        private:
            constexpr generator_iterator(T *coro) noexcept
                : coro{coro} {}

            T *coro;

            friend T;
        };
    }

    // A generator coroutine that yields the indices of the red and blue bars in the sorting algorithm.
    struct sorting_task final
    {
        sorting_task(sorting_task const &) = delete;
        sorting_task &operator=(sorting_task const &) = delete;

        inline sorting_task(sorting_task &&rhs) noexcept
            : handle{rhs.handle}
        {
            rhs.handle = nullptr;
        }

        inline sorting_task &operator=(sorting_task &&rhs) noexcept
        {
            if (this != &rhs)
            {
                if (handle)
                    handle.destroy();

                handle = rhs.handle;
                rhs.handle = nullptr;
            }

            return *this;
        }

        inline ~sorting_task() noexcept
        {
            if (handle)
                handle.destroy();
        }

        constexpr bool done() const noexcept { return !handle || handle.done(); }
        inline void resume() const noexcept { handle.resume(); }

        inline colors &value() noexcept { return handle.promise().cs; }
        inline colors const &value() const noexcept { return handle.promise().cs; }

        struct promise_type final
        {
            inline sorting_task get_return_object() noexcept
            {
                return {std::coroutine_handle<promise_type>::from_promise(*this)};
            }

            constexpr std::suspend_never initial_suspend() const noexcept { return {}; }
            constexpr std::suspend_always final_suspend() const noexcept { return {}; }

            inline void unhandled_exception() const { throw; }

            constexpr std::suspend_always yield_value(colors const &cs) noexcept
            {
                this->cs = cs;
                return {};
            }

            constexpr void return_void() const noexcept {}

            colors cs;
        };

        constexpr auto begin() noexcept { return detail::generator_iterator<sorting_task>{this}; }
        static constexpr auto end() noexcept { return std::default_sentinel; }

    private:
        inline sorting_task(std::coroutine_handle<promise_type> handle) noexcept
            : handle{handle} {}

        std::coroutine_handle<promise_type> handle;
    };
}