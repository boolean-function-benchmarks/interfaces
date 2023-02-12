import re
from os import path

import truth_table

class BenchmarkReader:
    """
    The BenchmarkReader class provides methods for validating and reading
    PLU and PLA benchmark files.

    The inputs and outputs are stored in an instance of the TruthTable class.
    """

    def __init__(self):
        # Constants for the status of the file format
        self.PLU = 0
        self.PLA = 1

        # Variables for the meta information of a certain benchmark
        self.num_inputs = -1
        self.num_outputs = -1
        self.num_chunks = -1

        # Create an TruthTable instance for the storage of the table data
        self.table = truth_table.TruthTable()

    def validate_file_path(self, file_path: str) -> None:
        """
        Validates the file which is adressed by the given path by checking
        whether the path is empty, the file exists, is a file and can be read.

        The file extension has to match those of plu and pla files (i.e. .plu .pla .PLU .PLA)

        :param file_path: Given path of the file which will be validated
        :return: None
        """

        # Check the length of the file path, raise Exception when empty
        if len(file_path) == 0:
            raise Exception("File path is empty!")

        # Validate whether the file exists
        if not path.exists(file_path):
            raise Exception("File does not exists!")

        # Check if the path addresses a file
        if not path.isfile(file_path):
            raise Exception("File path does not address a file!")

        # Get the file extension
        filename, extension = path.splitext(file_path)

        # Validate the extension by checking if it's a PLU or PLA file
        if not extension.lower() == '.plu' and not extension.lower() == '.pla':
            raise Exception("File type is not valid for benchmark reader!")

    def file_format(self, file_path: str) -> int:
        """
        Checks the file extension and returns the file format (PLU or PLA).

        :param file_path: Path of the file of which the extension will be checked
        :return: value of the respective file format constant
        """

        # Get the file extension
        filename, extension = path.splitext(file_path)

        # Check whether the extension is plu or pla
        if extension.lower() == '.plu':
            return self.PLU
        elif extension.lower() == '.pla':
            return self.PLA
        else:
            # Raise Exception is the file is not a PLU or PLA file
            raise Exception("Given file is not valid for benchmark reader!")

    def read_benchmark(self, file_path: str) -> None:
        """
        Reads a PLU or PLA benchmark file. First, the header is read and
        then the data of the compressed/uncompressed truth table is read line by line.

        The method automatically distinguishes between a PLU and PLA file.

        The procedures for the reading process differ in the header size and the format
        of each row of the truth table.

        :param file_path: Path of the PLU/PLA file
        :return: None
        """

        # First, validate the file path
        self.validate_file_path(file_path)

        # Determine the file format
        file_format = self.file_format(file_path)

        # Lists for the header and input/output rows
        header = []
        input_row = []
        output_row = []

        # Set header size depending on the status of the file format
        if file_format == self.PLU:
            header_size = 3
            self.table.compressed = True
        else:
            header_size = 2

        # Open the file for reading
        f = open(file_path, 'r')

        # Read in the header
        i = 0
        while i < header_size:
            line = f.readline()
            identifier, value = line.split()
            header.append(value)
            i += 1

        # Store the meta information of the header
        # in the respective member variables
        self.num_inputs = header[0]
        self.num_outputs = header[1]

        if file_format == self.PLU:
            self.num_chunks = header[2]

        # Read in the input/output data until the end of the
        # table data is reached
        line = f.readline()
        while '.e' not in line:
            # Distinguish the separation of the data of the current row
            # based on the file format
            if file_format == self.PLU:
                # Just perform the default split the current line by whitespace
                line_values = line.split()
            else:
                # Remove all whitespace from the current line
                line = re.sub(r"\s+", "", line)
                # Split it into a list
                line_values = list(line)

            # Iterate over the values of the current line
            for index, value in enumerate(line_values):
                # Depending on the number of inputs and outputs,
                # store them in the respective row vectors
                if index < int(self.num_inputs):
                    input_row.append(value)
                else:
                    output_row.append(value)

            # Store the data of the row in the vectors of the truth table
            self.table.append_inputs(input_row.copy())
            self.table.append_outputs(output_row.copy())

            # Remove the input/output row data from the temporary storage
            input_row.clear()
            output_row.clear()

            # Continue the iteration with the next line of the file
            line = f.readline()

        f.close()

    def print(self):
        """
        Calls the print method in the TruthTable class.

        :return: None
        """
        self.table.print()
