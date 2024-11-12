import random
import subprocess
from subprocess import PIPE
import sys

def generate():
    N = 1000000
    M = 1000
    ratio = M/N
    numbers = []
    outliers = []
    for i in range(0,N):
        numbers.append(str(i))
        if random.random() > ratio:
            numbers.append(str(i))
        else:
            outliers.append(str(i))
    
    random.shuffle(numbers)
        
    return (numbers, outliers)
    
def test(exec):
    numbers, outliers = generate()
    output = None
    result = subprocess.run(exec, input='\n'.join(numbers), stdout=PIPE, stderr=PIPE, text=True, check=True)
    if result.returncode == 0:
        decoded = set(result.stdout.strip().split('\n'))
        outliers = set(outliers)
        if outliers == decoded:
            print(f"Sets Match: Length - {len(decoded)}")
        else:
            # probably should dump output when mismatches
            print(f"Set Mismatch: Correct Length - {len(outliers)} | Returned Length - {len(decoded)}")
    else:
        print("Recieved returncode ", result.returncode)
        print(result.stderr)
    
test(sys.argv[1])