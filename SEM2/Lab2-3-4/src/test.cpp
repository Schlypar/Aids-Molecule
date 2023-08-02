
#include "Allocator.h"
#include "Sequence/ArraySequence.h"
#include <ADT.h>
#include <Logger.h>
#include <bits/ranges_algobase.h>
#include <iterator>
#include <stdexcept>
#include <tuple>

LogPriority Logger::priority = InfoPriority;

int square(int e)
{
	return e * e;
}

int main()
{

	std::vector<int> numbers = { 1, 2, 3, 4, 5 };
	std::vector<int> results;

	auto const squarer = output_transformer(square);

	std::copy(numbers.begin(), numbers.end(), squarer(std::back_inserter(results)));

	for (auto e : results)
		std::cout << e << " ";

	std::cout << '\n';

	return 0;
}
