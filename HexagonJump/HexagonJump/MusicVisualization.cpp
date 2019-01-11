#include "MusicVisualization.hpp"
#include "Utils.hpp"

namespace hexagon {
namespace {

void BinaryReverseTwoArrays(std::vector<double>& x, std::vector<double>& y)
{
	if (x.size() != y.size()) {
		throw std::runtime_error("x.size() != y.size()");
	}
	size_t j = 0u, n1, n2 = x.size() / 2;
	for (size_t i = 1u; i <= x.size() - 2; i++) {
		n1 = n2;
		while (j >= n1) {
			j -= n1;
			n1 /= 2;
		}
		j += n1;
		if (i < j) {
			std::swap(x[i], x[j]);
			std::swap(y[i], y[j]);
		}
	}
}

/**
In place Radix2 FFT based on https://cnx.org/exports/ce67266a-1851-47e4-8bfc-82eb447212b4%407.pdf/decimation-in-time-dit-radix-2-fft-7.pdf
Original author: Douglas L. Jones
Creative Commons "Attribution" license 1.0 http://creativecommons.org/licenses/by/1.0/
C++ implementation was rewritten and modified by me.
*/
void Radix2InPlace(std::vector<double>& real, std::vector<double>& imaginary, int exponent)
{
	size_t m = 1;
	for (int i = 0; i < exponent; i++) {
		auto a = 0.0;
		auto n = m;
		m *= 2;

		for (size_t j = 0; j < n; j++) {
			auto angle = -2 * PI * a / n;
			auto c = std::cos(angle);
			auto s = std::sin(angle);
			a += std::pow(2, exponent - i - 1);

			for (size_t k = j; k < real.size(); k = k + m) {
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
	if (real.size() != imaginary.size()) {
		throw std::runtime_error("real.size() != imaginary.size()");
	}
	auto exponent = std::log2f(static_cast<float>(real.size()));
	float intpart;

	if (std::modf(exponent, &intpart) != 0.f) {
		throw std::runtime_error("vectors's length is not power of two");
	}
	BinaryReverseTwoArrays(real, imaginary);
	Radix2InPlace(real, imaginary, (int)exponent);
}

void FetchSamples(const sf::Int16* samples,
	MusicVisualizationColumnData& samplesReal,
	MusicVisualizationColumnData& samplesImaginary,
	size_t samplesPerTick,
	size_t samplesCount,
	size_t& sampleIndex) 
{
	static constexpr double INT_16_MAX = 32768.0;

	for (size_t i = 0u; i < samplesPerTick && sampleIndex < samplesCount; i++) {
		samplesReal[i] = samples[sampleIndex] / INT_16_MAX;
		samplesImaginary[i] = samples[sampleIndex + 1] / INT_16_MAX;
		sampleIndex += 2;
	}
}

MusicVisualizationColumnData CountSpectrum(
	MusicVisualizationColumnData& samplesReal,
	MusicVisualizationColumnData& samplesImaginary,
	size_t samplesPerColumn,
	size_t numSpectrumColumns,
	double& avgSampleValue)
{
	MusicVisualizationColumnData visualizationColumnData(numSpectrumColumns);

	for (size_t i = 0u; i < numSpectrumColumns; i++) {
		double columnResult = 0;

		for (size_t j = 0u; j < samplesPerColumn; j++) {
			size_t index = j + i * samplesPerColumn;
			auto realMul = samplesReal[index] * samplesReal[index];
			auto imaginaryMul = samplesImaginary[index] * samplesImaginary[index];
			auto sampleResult = std::sqrt(realMul + imaginaryMul);

			columnResult += sampleResult;
			avgSampleValue += sampleResult;
		}
		visualizationColumnData[i] = columnResult / samplesPerColumn;
	}

	return visualizationColumnData;
}

void ClearSamples(
	MusicVisualizationColumnData& samplesReal,
	MusicVisualizationColumnData& samplesImaginary)
{
	std::fill(samplesReal.begin(), samplesReal.end(), 0.0);
	std::fill(samplesImaginary.begin(), samplesImaginary.end(), 0.0);
}

} // namespace

MusicVisualization CountMusicVisualizationData(
	const sf::SoundBuffer& buffer,
	float gameTimerate, 
	uint8_t numSpectrumColumns, 
	gui::ThreadSafeProgressBar& progressBar)
{
	auto samplesCount = buffer.getSampleCount();
	auto samplesPerTick = static_cast<size_t>(gameTimerate * buffer.getSampleRate());
	auto buffersSize = static_cast<size_t>(std::pow(2, static_cast<int>(std::log2(samplesPerTick) + 1)));
	auto samplesPerColumn = samplesPerTick / numSpectrumColumns;

	std::vector<double> samplesReal(buffersSize);
	std::vector<double> samplesImaginary(buffersSize);

	MusicVisualizationData visualizationData;
	visualizationData.reserve((samplesCount / samplesPerTick));

	double avgSampleValue = 0;

	for (size_t sampleIndex = 0u; sampleIndex < samplesCount;)
	{
		FetchSamples(buffer.getSamples(),
			samplesReal,
			samplesImaginary,
			samplesPerTick,
			samplesCount,
			sampleIndex);

		auto spectrum = CountSpectrum(samplesReal,
			samplesImaginary,
			samplesPerColumn,
			numSpectrumColumns,
			avgSampleValue);

		visualizationData.emplace_back(std::move(spectrum));
		ClearSamples(samplesReal, samplesImaginary);
		progressBar.UpdateProgress(static_cast<float>(sampleIndex) / samplesCount);
	}
	auto totalIterations = (samplesCount * numSpectrumColumns * samplesPerColumn);

	return { visualizationData, avgSampleValue / totalIterations };
}

}
