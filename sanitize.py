import sys

if __name__ == "__main__":
  if len(sys.argv) < 2:
    print "Too few arguments."
    sys.exit(0)
  
  f = open(sys.argv[1])
  r = open(sys.argv[1].split('.')[0].strip() + "_sanitized.ppm", 'w+');
  
  for line in f:
    entries = line.split(' ');
    
    for entry in entries:
      entry = entry.strip()
      if '239' not in entry and len(entry) > 0:
        r.write(entry + " ")
        
    r.write("\n")