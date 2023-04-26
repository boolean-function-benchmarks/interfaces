import benchmark_reader

# Create an instance of BenchmarkReader
reader = benchmark_reader.BenchmarkReader()

# Read the benchmark file
reader.read_benchmark('../../data/add1c.plu')

# Print the truth table
reader.print()
