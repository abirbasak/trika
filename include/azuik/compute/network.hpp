#ifndef AZUIK_COMPUTE_NETWORK_HPP
#define AZUIK_COMPUTE_NETWORK_HPP
#include <cmath>
namespace azuik
{
    namespace compute
    {
        struct exp_fn {
            template <class Tensor>
            auto constexpr operator()(Tensor const& x) const noexcept
            {
                return std::exp(x);
            }
        };
        inline static constexpr exp_fn exp = {};

        struct sigmoid_fn {
            template <class Tensor>
            auto constexpr operator()(Tensor const& x) const noexcept
            {
                return 1 / (1 + exp(-x));
            }
        };
        inline static constexpr sigmoid_fn sigmoid = {};

        struct dsigmoid_fn {
            template <class Tensor>
            auto constexpr operator()(Tensor const& x) const noexcept
            {
                return x * (1 - x);
            }
        };
        inline static constexpr dsigmoid_fn dsigmoid = {};

        struct tanh_fn {
            template <class Tensor>
            auto constexpr operator()(Tensor const& x) const noexcept
            {
                return std::tanh(x);
            }
        };

        inline static constexpr tanh_fn tanh = {};

        struct dtanh_fn {
            template <class Tensor>
            auto constexpr operator()(Tensor const& x) const noexcept
            {
                return 1 - x * x;
            }
        };
        inline static constexpr dtanh_fn dtanh = {};

    }; // namespace compute

} // namespace azuik
#endif
