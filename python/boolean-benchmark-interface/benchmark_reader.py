import re
from os import path
import array
import truth_table


#  TODO Integrity check -> Validate equal dimensions of inputs and outputs


class BenchmarkReader:
    """
    The BenchmarkReader class provides methods for validating and reading
    PLU and PLA benchmark files.

    The inputs and outputs are stored in an instance of the TruthTable class.
    """

    def __init__(self):
        # Constants for the status of the file format
        self.file = None
        self.PLU = 0
        self.PLA = 1
        self.TT = 2

        # Variables for the meta information of a certain benchmark
        self.num_inputs = -1
        self.num_outputs = -1
        self.num_chunks = -1
        self.num_product_terms = -1

        self.model_name = ""

        self.header_size = -1

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

        # Validate the extension by checking if it's a PLU file
        if not extension.lower() in ['.plu', '.pla', '.tt']:
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
        elif extension.lower() == '.tt':
            return self.TT
        else:
            # Raise Exception is the file is not a PLU or PLA file
            raise Exception("Given file is not valid for benchmark reader!")

    def open_file(self, file_path: str) -> None:
        """

        :param file_path:
        :return:
        """
        # First, validate the file path
        self.validate_file_path(file_path)

        self.file = open(file_path, 'r')

    def close_file(self, ) -> None:
        """
        :return:
        """
        self.file.close()

    def read_keyword(self, keyword: str) -> str:
        """

        :param keyword:
        :return:
        """

        if len(keyword) == 0:
            raise Exception("Keyword is empty!")

        self.file.seek(0)

        for line in self.file:
            if keyword in line:
                break
        else:
            # Just perform the default split the current line by whitespace
            line_values = line.split()

        # Just perform the default split the current line by whitespace
        line_values = line.split()

        if len(line_values) > 2:
            raise Exception("Not a valid keyword line")

        return line_values[1]

    def read_num_inputs(self) -> None:
        """

        :return:
        """
        self.num_inputs = self.read_keyword(".i")

    def read_num_outputs(self) -> None:
        """

        :return:
        """
        self.num_outputs = self.read_keyword(".o")

    def read_num_product_terms(self) -> None:
        """

        :return:
        """
        self.num_product_terms = self.read_keyword(".p")

    def read_model_name(self) -> None:
        """

        :return:
        """
        self.model_name = self.read_keyword(".model")

    def read_names(self, keyword: str) -> array:
        """

        :return:
        """

        if len(keyword) == 0:
            raise Exception("Keyword is empty!")

        self.file.seek(0)

        line_values = []

        for line in self.file:
            if keyword in line:
                line_values = line.split()
                break

        if len(line_values) <= 1:
            raise Exception("Not a valid keyword line")

        return line_values

    def read_input_names(self) -> None:
        """

        :return:
        """
        line_values = self.read_names(".ilb")
        self.table.input_names = list(line_values)
        self.table.input_names.pop()

    def read_output_names(self) -> None:
        """

        :return:
        """

        line_values = self.read_names(".ob")
        self.table.output_names = list(line_values)
        self.table.output_names.pop()

    def read_header(self) -> None:
        """

        :return:
        """

        self.header_size = 0

        self.read_num_inputs()

        if self.num_inputs != -1:
            self.header_size += 1

        self.read_num_outputs()

        if self.num_outputs != -1:
            self.header_size += 1

        self.read_model_name()

        if len(self.model_name) > 0:
            self.header_size += 1

        self.read_input_names()

        if len(self.table.input_names) > 0:
            self.header_size += 1

        self.read_output_names()

        if len(self.table.output_names) > 0:
            self.header_size += 1

    def print_header(self) -> None:
        """

        :return:
        """

        if len(self.model_name) > 0:
            print("Model: " + self.model_name)

        if self.num_inputs != -1:
            print("Inputs: " + self.num_inputs)

        if self.num_outputs != -1:
            print("Outputs: " + self.num_outputs)

        if len(self.table.input_names) > 0:
            self.table.print_input_names()

        if len(self.table.output_names) > 0:
            self.table.print_output_names()

    def read_tt_file(self, file_path: str) -> None:
        """

        :param file_path:
        :return:
        """

        # Determine the file format
        file_format = self.file_format(file_path)

        self.table.compressed = False
        self.file.seek(0)

        input_row = []
        output_row = []

        # Read in the input/output data until the end of the
        # table data is reached

        i = 0
        while i <= self.header_size:
            line = self.file.readline()
            i += 1

        while '.end' not in line:
            # Just perform the default split the current line by whitespace
            line_split = line.split()

            inputs = line_split[0]
            outputs = line_split[1]

            for index in range(len(inputs)):
                input_row.append(inputs[index])

            for index in range(len(outputs)):
                output_row.append(outputs[index])

            # Store the data of the row in the vectors of the truth table
            self.table.append_inputs(input_row.copy())
            self.table.append_outputs(output_row.copy())

            # Remove the input/output row data from the temporary storage
            input_row.clear()
            output_row.clear()

            # Continue the iteration with the next line of the file
            line = self.file.readline()

    def read_pla_file(self, file_path: str) -> None:
        """

        :param file_path:
        :return:
        """

    def read_plu_file(self, file_path: str) -> None:
        """
        Reads a PLU benchmark file. First, the header is read and
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
        header_size = 3
        self.table.compressed = True

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
        self.num_chunks = header[2]

        # Read in the input/output data until the end of the
        # table data is reached
        line = f.readline()
        while '.e' not in line:

            # Just perform the default split the current line by whitespace
            line_values = line.split()

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
