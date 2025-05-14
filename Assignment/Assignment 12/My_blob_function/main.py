import os

def main():
    # this is the blobs content
    blob_content = "This is the content of the blob."

    # create 'blobs' if we don't have
    os.makedirs("blobs", exist_ok=True)

    # write the information to blobs
    output_path = "blobs/output.txt"
    with open(output_path, "w") as f:
        f.write(blob_content)

    # show the screen
    print(f"Blob saved successfully at: {output_path}")

if __name__ == "__main__":
    main()
