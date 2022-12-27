class TruthTable:
    """
    Class for representing a compressed or uncompressed truth table.
    Input and outputs are stores row-wise in two-dimensional vectors.
    """

    def __init__(self):
        self.compressed = False
        self.inputs = []
        self.outputs = []

    def clear(self) -> None:
        """
        Clears the vectors of the input and output vectors.

        :return: None
        """
        self.inputs.clear()
        self.output.clear()

    def rows(self) -> int:
        """
        Return the number of rows of the table.

        :return: Number of rows.
        """
        return len(self.inputs)

    def append_inputs(self, input_row: list) -> None:
        """
        Appends a new input row vector to the inputs.
        Validates the input row vector by using assert.
        The input row vector must be not None and non-empty.

        :param input_row: Vector containing a row of inputs
        :return: None
        """
        assert input_row is not None and len(input_row) > 0, "Input row vector is" \
                                                         "None or empty!"
        self.inputs.append(input_row)

    def append_outputs(self, output_row: list) -> None:
        """
        Appends a new output row vector to the outputs.
        Validates the output row vector by using assert.
        The output row vector must be not None and non-empty.

        :param output_row: Vector containing a row of outputs
        :return: None
        """
        assert output_row is not None and len(output_row) > 0, "Output row vector is " \
                                                           "None or emtpy!"
        self.outputs.append(output_row)

    def get_inputs_at(self, index: int) -> list:
        """
        Returns a row of inputs at a specific index.
        Validates the index by using assert. The index
        must be in the interval 0 <= index <= max_index.

        :param index: index of the row
        :return: Vector containing a row of inputs
        """
        max_index = len(self.inputs)
        assert 0 <= index <= max_index, "Index is out of range!"
        return self.inputs[index]

    def get_outputs_at(self, index: int) -> list:
        """
        Returns a row of outputs at a specific index.
        Validates the index by using assert. The index
        must be in the interval 0 <= index <= max_index.

        :param index: index of the row
        :return: Vector containing a row of outputs
        """
        max_index = len(self.outputs)
        assert 0 <= index <= max_index, "Index is out of range!"
        return self.outputs[index]

    def print(self) -> None:
        """
        Print the table row-wise.

        :return: None
        """
        # Get the number of rows
        num_rows = self.rows()

        # Iterate over the number of rows
        for i in range(num_rows):

            # Print the inputs that are stored in the current
            # input row vector
            for input_row in self.inputs[i]:
                print(input_row + ' ', end='')

            # Separate inputs and outs with whitespace
            print("   ", end="")

            # Print the outputs of the current row then
            for output_row in self.outputs[i]:
                print(output_row + ' ', end='')

            print('')
