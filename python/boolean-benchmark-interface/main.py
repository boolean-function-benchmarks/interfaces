import benchmark_reader

# Create an instance of BenchmarkReader
reader = benchmark_reader.BenchmarkReader()

reader.open_file('test-data/add3.tt')

reader.read_header()

reader.print_header()

reader.read_tt_file('test-data/add3.tt')

# Print the truth table
reader.print()
