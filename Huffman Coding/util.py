import bitio
import huffman
import pickle


def read_tree(tree_stream):
    '''Read a description of a Huffman tree from the given compressed
    tree stream, and use the pickle module to construct the tree object.
    Then, return the root node of the tree itself.

    Args:
      tree_stream: The compressed stream to read the tree from.

    Returns:
      A Huffman tree root constructed according to the given description.
    '''

    tree = pickle.load(tree_stream)

    return tree

    pass


def decode_byte(tree, bitreader):
    """
    Reads bits from the bit reader and traverses the tree from
    the root to a leaf. Once a leaf is reached, bits are no longer read
    and the value of that leaf is returned.

    Args:
      bitreader: An instance of bitio.BitReader to read the tree from.
      tree: A Huffman tree.

    Returns:
      Next byte of the compressed bit stream.
    """

    # assume that a leaf is not found 
    found = False

    # dig into tree till the leaf is found
    while not found:

    	#read in exactly one bit
    	code = bitreader.readbit()

    	# 1 implies go right and 0 implies go left
    	if code == 1:
    		tree = tree.getRight()
    	elif code == 0:
    		tree = tree.getLeft()

    	# if leaf is found, stop loop and return leaf value
    	if isinstance(tree, huffman.TreeLeaf):
    		found = True
    		return tree.getValue()
    	
    pass


def decompress(compressed, uncompressed):
    '''First, read a Huffman tree from the 'compressed' stream using your
    read_tree function. Then use that tree to decode the rest of the
    stream and write the resulting symbols to the 'uncompressed'
    stream.

    Args:
      compressed: A file stream from which compressed input is read.
      uncompressed: A writable file stream to which the uncompressed
          			output is written.
    '''

    # create tree using read_tree
    try:
    	tree = read_tree(compressed)
    except:
    	print("Huffman tree description invalid")
    	return

    # open using bitio
    bitread = bitio.BitReader(compressed)
    bitwrite = bitio.BitWriter(uncompressed)

    # attempt to decode file bits after Huffman tree description
    try:
    	decode = decode_byte(tree, bitread)
    except EOFError:
    	decode = None
    	print ("No bits to read after Huffman tree description")

    # if bits are successfully decoded, begin writing them, till EOF is reached
    while decode != None:

    	try:
    		# write decoded symbol with 8 bits and decode the next bits 
    		bitwrite.writebits(decode, 8)
    		decode = decode_byte(tree, bitread)

    	# exception in case compressed file does not have None written before EOF
    	except EOFError:
    		decode = None

    pass


def write_tree(tree, tree_stream):
    '''Write the specified Huffman tree to the given tree_stream
    using pickle.

    Args:
      tree: A Huffman tree.
      tree_stream: The binary file to write the tree to.
    '''

    pickle.dump(tree, tree_stream)

    pass


def compress(tree, uncompressed, compressed):
    '''First write the given tree to the stream 'compressed' using the
    write_tree function. Then use the same tree to encode the data
    from the input stream 'uncompressed' and write it to 'compressed'.
    If there are any partially-written bytes remaining at the end,
    write 0 bits to form a complete byte.

    Flush the bitwriter after writing the entire compressed file.

    Args:
      tree: A Huffman tree.
      uncompressed: A file stream from which you can read the input.
      compressed: A file stream that will receive the tree description
          and the coded input data.
    '''

    # no try statement given, becuase if file exists, and 
    try:
    	write_tree(tree, compressed)
    except:
    	print("File does not contain any bits to compress")
    	return
    
    encoding_table = huffman.make_encoding_table(tree)

    # open using bitio
    bitread = bitio.BitReader(uncompressed)
    bitwrite = bitio.BitWriter(compressed)

    # assume EOF is not reached
    eof = False

    # read till EOF is reached
    while not eof:

    	try:
    		# read 8 bits to byte
    		byte = bitread.readbits(8)
    		# encode using encoding_table
    		code = encoding_table[byte]
    		# write code bitwise 
    		for bit in code:
    			bitwrite.writebit(bit)

    	# this is a crucial step to ensure loop stops 
    	# and compressed files are marked with None to indicate EOF
    	except EOFError:
    		# stop loop
    		eof = True

    		# manually write None to indicate EOF to decompressor 
    		code = encoding_table[None]
    		for bit in code:
    			bitwrite.writebit(bit)

    bitwrite.flush()

    pass
