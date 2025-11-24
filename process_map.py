import sys
import os

def process_files(input_filename):
    # 1. Determine file paths
    wakable_file_path = input_filename
    
    base_name, extension = os.path.splitext(wakable_file_path)
    resource_file_path = f"{base_name}_resources{extension}"
    
    output_file_path = f"processed_{os.path.basename(input_filename)}"

    print(f"Reading Wakable Info File: {wakable_file_path}")
    print(f"Reading Resource File: {resource_file_path}")

    # 2. Read the files
    try:
        with open(wakable_file_path, 'r') as w_file:
            wakable_lines = [line.strip() for line in w_file if line.strip()]
            
        with open(resource_file_path, 'r') as r_file:
            resource_lines = [line.strip() for line in r_file if line.strip()]
    except FileNotFoundError as e:
        print(f"Error: {e}")
        print("Make sure both files exist in the directory.")
        return

    # 3. Validate Dimensions
    if len(wakable_lines) != len(resource_lines):
        print(f"Error: Row mismatch. Wakable file has {len(wakable_lines)} rows, Resource file has {len(resource_lines)}.")
        return

    processed_matrix = []

    # 4. Process the matrix
    for row_idx in range(len(wakable_lines)):
        # Convert strings to lists of characters (handling dense strings like "0012")
        w_row_chars = list(wakable_lines[row_idx])
        r_row_chars = list(resource_lines[row_idx])

        # Filter out anything that isn't a digit (just in case of weird whitespace)
        w_row_chars = [c for c in w_row_chars if c.isdigit()]
        r_row_chars = [c for c in r_row_chars if c.isdigit()]

        if len(w_row_chars) != len(r_row_chars):
            print(f"Error: Column mismatch at row {row_idx}.")
            return

        new_row = []
        
        for col_idx in range(len(w_row_chars)):
            h_val = int(w_row_chars[col_idx])
            r_val = int(r_row_chars[col_idx])
            
            final_val = -1

            # --- LOGIC RULES ---
            # Priority 1: Is it a resource?
            if r_val == 1:
                final_val = 2
            else:
                # Priority 2: Wakable Logic
                if h_val == 1:
                    final_val = 0 # Non Walkable / Non Buildable
                elif h_val == 0 or h_val == 2:
                    final_val = 1 # Wakable and / or Buildable
                else:
                    # Fallback for unexpected values (treat as non walkable)
                    final_val = 0 
            
            new_row.append(str(final_val))
        
        # Join the list back into a string (e.g., "10021")
        processed_matrix.append("".join(new_row))

    # 5. Write the result
    with open(output_file_path, 'w') as out_file:
        for line in processed_matrix:
            out_file.write(line + "\n")

    print(f"Success! Processed matrix written to: {output_file_path}")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python script.py <filename>")
        print("Example: python script.py python_1.1.txt")
    else:
        process_files(sys.argv[1])