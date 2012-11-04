TARGET=lambda_input


all:
	gcc -ggdb ./src/lambda.c ./src/lambda_dict.c ./src/lambda_trie.c -o $(TARGET) -I ./include

clean:
	rm -f $(TARGET)
