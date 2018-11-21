#pragma once

#include "FFT.hpp"
#include "Utils.hpp"

namespace hexagon::fft {

static void BinaryReverseTwoArrays(std::vector<double>& x, std::vector<double>& y)
{
	if (x.size() != y.size())
	{
		throw std::runtime_error("x.size() != y.size()");
	}
	size_t j = 0u, n1, n2 = x.size() / 2;
	for (size_t i = 1u; i <= x.size() - 2; i++)
	{
		n1 = n2;
		while (j >= n1)
		{
			j -= n1;
			n1 /= 2;
		}
		j += n1;
		if (i < j)
		{
			std::swap(x[i], x[j]);
			std::swap(y[i], y[j]);
		}
	}
}

static void Radix2InPlace(std::vector<double>& real, std::vector<double>& imaginary, int exponent)
{
	size_t m = 1;
	for (int i = 0; i < exponent; i++)
	{
		double a = 0.0;
		size_t n = m;
		m *= 2;

		for (size_t j = 0; j < n; j++)
		{
			auto c = std::cos(a);
			auto s = std::sin(a);
			a += -2 * utils::pi() / m;
			for (size_t k = j; k < real.size(); k = k + m)
			{
				auto t1 = c * real[k + n] - s * imaginary[k + n];
				auto t2 = s * real[k + n] + c * imaginary[k + n];
				real[k + n] = real[k] - t1;
				imaginary[k + n] = imaginary[k] - t2;
				real[k] += t1;
				imaginary[k] += t2;
			}
		}
	}
}

void FFT(std::vector<double>& real, std::vector<double>& imaginary)
{
	if (real.size() != imaginary.size())
	{
		throw std::runtime_error("real.size() != imaginary.size()");
	}
	auto exponent = std::log2f(real.size());
	float intpart;
	if (std::modf(exponent, &intpart) != 0.f)
	{
		throw std::runtime_error("Vectors's length is not power of two");
	}
	BinaryReverseTwoArrays(real, imaginary);
	Radix2InPlace(real, imaginary, (int)exponent);
}

}