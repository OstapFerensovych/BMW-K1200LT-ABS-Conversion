#pragma once
#include "gpio.hpp"

enum class line {
    __invalid__,
    can_rx,
    can_tx,
    speedo,
    gen_warn,
    abs_warn_relay,
    brake_lights,
    tail_lights,
    gpio_A0,
    gpio_A1,
    gpio_A2,
    gpio_F0,
    gpio_F1
};

// clang-format off
constexpr std::pair<line, gpio::pin_config> pins_config[] = {
    {line::can_rx,         {gpio::port::a, 11, 0, gpio::mode::af_io, gpio::drive::push_pull, gpio::speed::high, gpio::pull::none, gpio::alt_func::af4}},
    {line::can_tx,         {gpio::port::a, 12, 0, gpio::mode::af_io, gpio::drive::push_pull, gpio::speed::high, gpio::pull::none, gpio::alt_func::af4}},
    {line::speedo,         {gpio::port::a, 7, 0,  gpio::mode::af_io,  gpio::drive::push_pull, gpio::speed::high, gpio::pull::none, gpio::alt_func::af1 }},
    {line::gen_warn,       {gpio::port::a, 6, 0,  gpio::mode::gp_out,  gpio::drive::push_pull, gpio::speed::low, gpio::pull::none, gpio::alt_func::none }},
    {line::abs_warn_relay, {gpio::port::a, 5, 1,  gpio::mode::gp_out,  gpio::drive::push_pull, gpio::speed::low, gpio::pull::none, gpio::alt_func::none }},
    {line::brake_lights,   {gpio::port::a, 4, 1,  gpio::mode::gp_out,  gpio::drive::push_pull, gpio::speed::low, gpio::pull::none, gpio::alt_func::none }},
    {line::tail_lights,    {gpio::port::a, 3, 1,  gpio::mode::gp_out,  gpio::drive::push_pull, gpio::speed::low, gpio::pull::none, gpio::alt_func::none }},
    {line::gpio_A0,        {gpio::port::a, 0, 0,  gpio::mode::gp_out,  gpio::drive::push_pull, gpio::speed::low, gpio::pull::none, gpio::alt_func::none }},
    {line::gpio_A1,        {gpio::port::a, 1, 0,  gpio::mode::gp_out,  gpio::drive::push_pull, gpio::speed::low, gpio::pull::none, gpio::alt_func::none }},
    {line::gpio_A2,        {gpio::port::a, 2, 0,  gpio::mode::gp_out,  gpio::drive::push_pull, gpio::speed::low, gpio::pull::none, gpio::alt_func::none }},
    {line::gpio_F0,        {gpio::port::f, 0, 0,  gpio::mode::gp_out,  gpio::drive::push_pull, gpio::speed::low, gpio::pull::none, gpio::alt_func::none }},
    {line::gpio_F1,        {gpio::port::f, 1, 0,  gpio::mode::gp_out,  gpio::drive::push_pull, gpio::speed::low, gpio::pull::none, gpio::alt_func::none }},
};
// clang-format on
