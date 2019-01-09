#include "MusicVisualization.hpp"
#include "Utils.hpp"

namespace hexagon {
namespace {

void BinaryReverseTwoArrays(std::vector<double>& x, std::vector<double>& y)
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

/**
In place Radix2 FFT based on https://cnx.org/exports/ce67266a-1851-47e4-8bfc-82eb447212b4%407.pdf/decimation-in-time-dit-radix-2-fft-7.pdf
Original author: Douglas L. Jones
Creative Commons "Attribution" license 1.0 http://creativecommons.org/licenses/by/1.0/
C++ implementation was done by me.
*/
void Radix2InPlace(std::vector<double>& real, std::vector<double>& imaginary, int exponent)
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
			a += -2 * PI / m;
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
	auto exponent = std::log2f(static_cast<float>(real.size()));
	float intpart;
	if (std::modf(exponent, &intpart) != 0.f)
	{
		throw std::runtime_error("vectors's length is not power of two");
	}
	BinaryReverseTwoArrays(real, imaginary);
	Radix2InPlace(real, imaginary, (int)exponent);
}

} // namespace

MusicVisualization CountMusicVisualizationData(const sf::SoundBuffer& buffer,
	float gameTimerate, 
	uint8_t spectrumColumns, 
	gui::ThreadSafeProgressBar& progressBar)
{
	// TODO SIMPLIFY
	auto samples = buffer.getSamples();
	auto sampleRate = buffer.getSampleRate();
	auto sampleCount = buffer.getSampleCount();
	auto samplesPerTick = static_cast<size_t>(gameTimerate * sampleRate);
	auto samplesPerTickPowerOfTwo = static_cast<size_t>(std::pow(2, static_cast<int>(std::log2(samplesPerTick) + 1)));
	auto samplesPerColumn = samplesPerTick / spectrumColumns;
	double avgSampleValue = 0;
	std::vector<double> samplesReal(samplesPerTickPowerOfTwo), samplesImaginary(samplesPerTickPowerOfTwo);
	MusicVisualizationData visualizationData;
	visualizationData.reserve((sampleCount / samplesPerTick));

	for (size_t sampleIndex = 0u; sampleIndex < sampleCount;)
	{
		// Fetch samples
		for (size_t i = 0u; i < samplesPerTick; i++) {
			if (sampleIndex >= sampleCount) {
				break;
			}
			samplesReal[i] = samples[sampleIndex] / 32768.0;
			samplesImaginary[i] = samples[sampleIndex + 1] / 32768.0;
			sampleIndex += 2;
		}

		// Count spectrum
		MusicVisualizationColumnData visualizationColumnData(spectrumColumns);

		for (size_t i = 0u; i < spectrumColumns; i++) {
			double columnResult = 0;
			for (size_t j = 0u; j < samplesPerColumn; j++) {
				size_t index = j + i * samplesPerColumn;
				auto sampleResult = std::sqrt(samplesReal[index] * samplesReal[index] + samplesImaginary[index] * samplesImaginary[index]);
				columnResult += sampleResult;
				avgSampleValue += sampleResult;
			}
			visualizationColumnData[i] = columnResult / samplesPerColumn;
		}

		// Save it
		visualizationData.emplace_back(std::move(visualizationColumnData));

		// Clear for further use
		std::fill(samplesReal.begin(), samplesReal.end(), 0.0);
		std::fill(samplesImaginary.begin(), samplesImaginary.end(), 0.0);

		// Update progress bar
		progressBar.UpdateProgress(static_cast<float>(sampleIndex) / sampleCount);
	}

	return { visualizationData, avgSampleValue / (sampleCount * spectrumColumns * samplesPerColumn) };
}

}
