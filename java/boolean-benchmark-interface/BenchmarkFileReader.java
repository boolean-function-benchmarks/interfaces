import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.text.NumberFormat;
import java.text.ParseException;
import java.util.ArrayList;
import java.util.Iterator;
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
 * @version 1.0
 * 
 * 
 */
public class BenchmarkFileReader {

	private TruthTable<Number> table = new TruthTable<Number>();

	// Constants to distinguish between PLU and PLA file
	public static final int PLU = 0;
	public static final int PLA = 1;
	public static final int TT = 2;

	private int numInputs = -1;
	private int numOutputs = -1;
	private int numProductTerms = -1;
	private int numChunks;
	private int headerSize;

	private String modelName = "";

	private Vector<String> inputNames = new Vector<String>();
	private Vector<String> outputNames = new Vector<String>();

	private String filePath;

	RandomAccessFile randomAccessFile;

	/**
	 * 
	 * @param keyword
	 * @return
	 * @throws IOException
	 * @throws RuntimeException
	 */
	public String readKeyword(String keyword) throws IOException, RuntimeException {

		String line;

		this.randomAccessFile.seek(0);

		while ((line = this.randomAccessFile.readLine()) != null) {
			if (line.contains(keyword)) {
				String[] lineSplit = line.split(" ");

				if (lineSplit.length != 2) {
					throw new RuntimeException("Not a valid keyword line!");
				} else {
					return lineSplit[1];
				}
			}
		}
		return "";
	}

	/**
	 * 
	 */
	public void readNumInputs() {

		try {
			String inputs = this.readKeyword(".i");
			if (inputs.length() > 0) {
				this.numInputs = Integer.parseInt(inputs);
			}
		} catch (IOException e) {
			e.printStackTrace();
		} catch (RuntimeException e) {
			e.printStackTrace();
		}

	}

	/**
	 * 
	 */
	public void readNumOutputs() {

		try {
			String outputs = this.readKeyword(".o");
			if (outputs.length() > 0) {
				this.numOutputs = Integer.parseInt(outputs);
			}
		} catch (IOException e) {
			e.printStackTrace();
		} catch (RuntimeException e) {
			e.printStackTrace();
		}

	}

	/**
	 * 
	 */
	public void readNumProductTerms() {
		try {
			String productTerms = this.readKeyword(".p");
			if (productTerms.length() > 0) {
				this.numProductTerms = Integer.parseInt(productTerms);
			}
		} catch (IOException e) {
			e.printStackTrace();
		} catch (RuntimeException e) {
			e.printStackTrace();
		}
	}

	/**
	 * 
	 */
	public void readModelName() {
		try {
			String model = this.readKeyword(".model");
			this.modelName = model;
		} catch (IOException e) {
			e.printStackTrace();
		} catch (RuntimeException e) {
			e.printStackTrace();
		}
	}

	/**
	 * 
	 */
	public void readNames(String keyword, Vector<String> names) throws IOException {
		String line;
		this.randomAccessFile.seek(0);
		while ((line = this.randomAccessFile.readLine()) != null) {
			if (line.contains(keyword)) {
				String[] lineSplit = line.split(" ");

				for (int i = 1; i < lineSplit.length; i++) {
					names.add(lineSplit[i]);
				}

				break;
			}
		}
	}

	/**
	 * 
	 */
	public void readInputNames() {
		try {
			this.readNames(".ilb", this.inputNames);
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	/**
	 * 
	 */
	public void readOutputNames() {
		try {
			this.readNames(".ob", this.outputNames);
		} catch (IOException e) {
			e.printStackTrace();
		}

	}

	/**
	 * 
	 */
	public void readHeader() {

		this.headerSize = 0;

		this.readNumInputs();

		if (this.numInputs > 0) {
			this.headerSize++;
		}

		this.readNumOutputs();

		if (this.numOutputs > 0) {
			this.headerSize++;
		}

		this.readModelName();

		if (this.modelName.length() > 0) {
			this.headerSize++;
		}

		this.readNumProductTerms();

		if (this.numProductTerms > 0) {
			this.headerSize++;
		}

		this.readInputNames();

		if (this.inputNames.size() > 0) {
			this.table.setInputNames(this.inputNames);
			this.headerSize++;
		}

		this.readOutputNames();

		if (this.outputNames.size() > 0) {
			this.table.setOutputNames(this.outputNames);
			this.headerSize++;
		}
	}

	/**
	 * 
	 */
	public void printHeader() {

		if (this.modelName.length() > 0) {
			System.out.println("Model:" + this.modelName);
		}

		if (this.numInputs > 0) {
			System.out.println("Inputs: " + this.numInputs);
		}

		if (this.numOutputs > 0) {
			System.out.println("Outputs: " + this.numOutputs);
		}

		if (this.numProductTerms > 0) {
			System.out.println("Product terms: " + this.numProductTerms);
		}

		if (this.table.getInputNames().size() > 0) {
			System.out.print("Input names:");
			this.table.printInputNames();
			System.out.println();
		}

		if (this.table.getOutputNames().size() > 0) {
			System.out.print("Output names:");
			this.table.printOutputNames();
			System.out.println();
		}

	}

	/**
	 * 
	 * @param filePath
	 */
	private void openFile(String filePath) {

		try {
			File file = new File(filePath);
			this.randomAccessFile = new RandomAccessFile(file, "r");
			this.filePath = filePath;
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	/**
	 * 
	 */
	private void closeFile() {
		try {
			this.randomAccessFile.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

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

		if (!(fileExtension.equals("plu"))) {
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

		if (fileExtension.equals("plu")) {
			return PLU;
		} else if (fileExtension.equals("pla")) {
			return PLA;
		} else if (fileExtension.equals("tt")) {
			return TT;
		} else {
			throw new RuntimeException("Invalid file format!");
		}

	}

	public void readTTFile(String filePath) throws IOException, ParseException {

		if (this.randomAccessFile == null) {
			openFile(filePath);
		}

		// Declaration and instantiation of chunk input/output vectors
		Vector<Number> inputRow = new Vector<Number>();
		Vector<Number> outputRow = new Vector<Number>();

		String line;

		// Each line read will be split and store in this array
		String[] substrs;

		Number value;
		int val;

		// Clear all rows/chunks of the table
		table.clear();
		this.randomAccessFile.seek(0);

		for (int i = 0; i < this.headerSize; i++) {
			this.randomAccessFile.readLine();
		}

		// Read in the first line
		line = this.randomAccessFile.readLine();

		// Repeat until the end of the table is reached
		while (!line.equals(".end")) {

			substrs = line.trim().split("\\s+");

			for (int i = 0; i < this.numInputs; i++) {
				val = Integer.parseInt("" + substrs[0].charAt(i));
				inputRow.add(val);
			}

			for (int i = 0; i < this.numOutputs; i++) {
				val = Integer.parseInt("" + substrs[1].charAt(i));
				outputRow.add(val);
			}
			line = this.randomAccessFile.readLine();
			
			table.addInputs(inputRow);
			table.addOutputs(outputRow);

			inputRow.clear();
			outputRow.clear();
		}

		line = this.randomAccessFile.readLine();
	}

	public void readPLAFile(String filePath) throws IOException, ParseException {

	}

	/**
	 * Interprets and reads a PLU benchmark file. The header is read first. The
	 * header contains the number of inputs (.i) and outputs (.o). The header of an
	 * PLU file additionally contains the number of chunk (.p).
	 * 
	 * The rows/chunks which contain the respective input/output pairs of a
	 * compressed truth table are read line by line. The procedure then splits the
	 * lines according to the respective file format and stores the input/outputs in
	 * chunk vectors.
	 * 
	 * 
	 * @param filePath path of the benchmark file
	 * @see TruthTable
	 * 
	 * @throws IOException
	 * @throws ParseException
	 */
	@SuppressWarnings("unchecked")
	public void readPLUFile(String filePath) throws IOException, ParseException {

		// Validate the path and respective file first
		validateFile(filePath);

		if (this.randomAccessFile == null) {
			openFile(filePath);
		}

		// Declaration and instantiation of chunk input/output vectors
		Vector<Number> inputChunk = new Vector<Number>();
		Vector<Number> outputChunk = new Vector<Number>();

		String line;

		// Each line read will be split and store in this array
		String[] substrs;

		Number value;

		int headerSize = 3;

		int header[] = new int[headerSize];
		int pos;

		// Clear all rows/chunks of the table
		table.clear();

		// Read the header from the file
		for (int i = 0; i < headerSize; i++) {
			line = this.randomAccessFile.readLine();
			pos = line.lastIndexOf(" ");
			header[i] = Integer.parseInt(line.substring(pos + 1));
		}

		// Sore the header in the respective member variables
		this.numInputs = header[0];
		this.numOutputs = header[1];

		// For the PLU file additionally store the number of chunks
		this.numChunks = header[2];

		// Read in the first line
		line = this.randomAccessFile.readLine();

		// Repeat until the end of the table is reached
		while (!line.equals(".e")) {

			// Distinguish the split of the current line
			// For PLU files, the whitespace is sued for splitting since
			// every variable is separated by at least one whitespace
			substrs = line.trim().split("\\s+");

			for (int i = 0; i < substrs.length; i++) {

				value = Long.parseLong(substrs[i]);

				if (i < numInputs) {
					inputChunk.add(value);
				} else {
					outputChunk.add(value);
				}
			}

			table.addInputs(inputChunk);
			table.addOutputs(outputChunk);

			inputChunk.clear();
			outputChunk.clear();

			line = this.randomAccessFile.readLine();
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
	 * Returns the truth table of the class. *
	 * 
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
		return numInputs;
	}

	/**
	 * Returns the number of outputs.
	 * 
	 * @return number of outputs.
	 */
	public int getOutputs() {
		return numOutputs;
	}

	/**
	 * Returns the number chunks of a compressed truth table
	 * 
	 * @return number of chunks
	 */
	public int getChunks() {
		return numChunks;
	}

	public static void main(String[] args) {

		// Instantiate new reader object
		BenchmarkFileReader reader = new BenchmarkFileReader();

		reader.openFile("../test-data/add3.tt");
		reader.readHeader();
		reader.printHeader();

		try {
			reader.readTTFile("../test-data/add3.tt");
		} catch (IOException | ParseException e) {
			e.printStackTrace();
		}

		// Read a benchmark file from the test data folder
		// reader.readPLUFile("../test-data/add2.pla");

		// Print the respective table
		reader.printTable();

	}

}
