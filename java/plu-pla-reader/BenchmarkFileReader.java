import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.text.NumberFormat;
import java.text.ParseException;
import java.util.Vector;

/**
 * Implements the file reader for PLU and PLA files. The given file will be
 * validated and the respective formated will be determined. Based on the format
 * of the file the reading procedure is chosen.
 * 
 * The data of compressed (PLU files) and uncompressed (PLA files) tables is
 * stored in an object of the class TruthTable.
 * 
 * 
 * @author Roman Kalkreuth, https://orcid.org/0000-0003-1449-5131,
 *         https://www.researchgate.net/profile/Roman-Kalkreuth,
 *         https://twitter.com/RomanKalkreuth
 */
public class BenchmarkFileReader {

	private TruthTable<Number> table = new TruthTable<Number>();

	// Constants to distinguish between PLU and PLA file
	public static final int PLU = 0;
	public static final int PLA = 1;

	private int inputs;
	private int outputs;
	private int chunks;

	/**
	 * Validates whether the file exists, can be read and has a valid extension (plu
	 * or pla).
	 * 
	 * @param filePath path to the benchmark file that should be read
	 * 
	 * @throws IllegalArgumentException Exception is thrown when the file path is
	 *                                  empty or has an invalid file extension.
	 * 
	 * @throws IOException              Exception is thrown when the file does not
	 *                                  exists or can't be read.
	 */
	private void validateFile(String filePath) throws IllegalArgumentException, IOException {

		// Validate the length of the file path
		if (filePath.length() == 0) {
			throw new IllegalArgumentException("File path is emtpy!");
		}

		File file = new File(filePath);

		// Validate whether the file exists and can be read

		if (!file.exists()) {
			throw new IOException("File dows not exist!");
		}

		if (!file.canRead()) {
			throw new IOException("File can't be read!");
		}

		// Finally, extract and validate the file extension
		String fileName = file.getName();
		String fileExtension = fileName.substring(fileName.lastIndexOf(".") + 1);

		if (!(fileExtension.equals("plu") || fileExtension.equals("pla"))) {
			throw new IllegalArgumentException("File must be a plu or pla-file!");
		}

	}

	/**
	 * Checks and returns one of the file format flags which have been declared has
	 * constants in this class (PLU and PLA).
	 * 
	 * 
	 * @param filePath path of the benchmark file
	 * @return flag for the file format (0 for PLU, 1 for PLA)
	 */
	private int getFormat(String filePath) {

		File file = new File(filePath);

		// Extract the filename from the path
		String fileName = file.getName();

		// Get the file extension.
		String fileExtension = fileName.substring(fileName.lastIndexOf(".") + 1);
		
		// Convert string to lowercase 
		fileExtension.toLowerCase();

		// Return the respective extension flag
		return (fileExtension.equals("plu")) ? PLU : PLA;

	}

	/**
	 * Interprets and reads the benchmark file. This method automatically
	 * distinguishes between PLU and PLA file. The header is read first. The header
	 * contains the number of inputs (.i) and outputs (.o). The header of an PLU
	 * file additionally contains the number of chunk (.p).
	 * 
	 * The rows/chunks which contain the respective input/output pairs of a
	 * compressed or uncompressed truth table are read line by line. The procedure
	 * then splits the lines according to the respective file format and stores the
	 * input/outputs in chunk vectors.
	 * 
	 * 
	 * @param filePath path of the benchmark file
	 * 
	 * @throws IOException
	 * @throws ParseException
	 */
	@SuppressWarnings("unchecked")
	public void readFile(String filePath) throws IOException, ParseException {

		// Validate the path and respective file first
		validateFile(filePath);

		FileReader fr = new FileReader(filePath);
		BufferedReader br = new BufferedReader(fr);

		// Declaration and instantiation of chunk input/output vectors
		Vector<Number> inputChunk = new Vector<Number>();
		Vector<Number> outputChunk = new Vector<Number>();

		String line;
		
		// Each line read will be split and store in this array
		String[] substrs;

		Number value;

		int headerSize;
		
		// Determine the file format
		int fileFormat = this.getFormat(filePath);

		// Distinguish the header size of a PLU and PLA file
		if (fileFormat == PLU) {
			headerSize = 3;
			// PLU files have compressed truth tables
			table.setCompressed(true);
		} else {
			headerSize = 2;
		}

		int header[] = new int[headerSize];
		int pos;

		// Clear all rows/chunks of the table
		table.clear();

		// Read the header from the file
		for (int i = 0; i < headerSize; i++) {
			line = br.readLine();
			pos = line.lastIndexOf(" ");
			header[i] = Integer.parseInt(line.substring(pos + 1));
		}

		// Sore the header in the respective member variables
		this.inputs = header[0];
		this.outputs = header[1];

		// For the PLU file additionally store the number of chunks
		if (fileFormat == PLU) {
			this.chunks = header[2];
		}

		// Read in the first line
		line = br.readLine();

		// Repeat until the end of the table is reached
		while (!line.equals(".e")) {

			// Distinguish the split of the current line
			if (fileFormat == PLU) {

				// For PLU files, the whitespace is sued for splitting since
				// every variable is separated by at least one whitespace
				substrs = line.trim().split("\\s+");

			} else {
				// In PLA files not every input/output variable is separated by whitespace.
				// Therefore, first clear the line of whitespace and then separate each char.
				line = line.replaceAll("\\s+", "");
				substrs = line.split("");
			}

			// 
			for (int i = 0; i < substrs.length; i++) {

				if (fileFormat == PLU) {
					value = Long.parseLong(substrs[i]);
				} else {
					value = Byte.parseByte(substrs[i]);
				}

				if (i < inputs) {
					inputChunk.add(value);
				} else {
					outputChunk.add(value);
				}
			}

			table.addInputs(inputChunk);
			table.addOutputs(outputChunk);

			inputChunk.clear();
			outputChunk.clear();

			line = br.readLine();
		}
	}

	/**
	 * Prints a compressed or uncompressed truth table by calling the print method
	 * of the TruthTable class.
	 */
	public void printTable() {
		table.print();
	}

	/**
	 * Returns the truth table of the class. 
	 * 	 * 
	 * @return truth table 
	 */
	public TruthTable<Number> getTable() {
		return table;
	}

	/**
	 * Returns the number of inputs.
	 * 
	 * @return number of inputs.
	 */
	public int getInputs() {
		return inputs;
	}

	/**
	 * Returns the number of outputs.
	 * 
	 * @return number of outputs.
	 */
	public int getOutputs() {
		return outputs;
	}

	/**
	 * Returns the number chunks of a compressed truth table
	 * 
	 * @return number of chunks
	 */
	public int getChunks() {
		return chunks;
	}

	public static void main(String[] args) {


		// Instantiate new reader object
		BenchmarkFileReader reader = new BenchmarkFileReader();

		try {
			// Read a benchmark file from the test data folder  
			reader.readFile("../test-data/add2.pla");
			
			// Print the respective table 
			reader.printTable();
		} catch (IOException e) {
			e.printStackTrace();
		} catch (ParseException e) {
			e.printStackTrace();
		}

	}

}
