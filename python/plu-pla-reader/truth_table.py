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

        :param input_row: Vector containing a row of inputs
        :return: None
        """
        self.inputs.append(input_row)

    def append_outputs(self, output_row: list) -> None:
        """
        Appends a new output row vector to the outputs.

        :param output_row: Vector containing a row of outputs
        :return: None
        """
        self.inouts.append(output_row)

    def get_inputs_at(self, index: int) -> list:
        """
        Returns a row of inputs at a specific index.

        :param index: index of the row
        :return: Vector containing a row of inputs
        """
        return self.inputs[index]

    def get_outputs_at(self, index: int) -> list:
        """
        Returns a row of outputs at a specific index.

        :param index: index of the row
        :return: Vector containing a row of outputs
        """
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
