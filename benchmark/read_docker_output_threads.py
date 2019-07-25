import os
import pandas as pd
import sys

get_headline=False
headline=""
first_line=""
#if not os.path.isfile(out_file):
get_headline=True

if(len(sys.argv)!=2):
	print("Please provide path to csv table")
	sys.exit(1);
path=sys.argv[1]

this_csv=""

for filename in os.listdir(path):
		parts=filename.split("_")
		#if parts[0][0]=="m":
			
		line_started=False
		try:
			with open(path+"/"+filename) as f:
				for line in f:
					line=line.strip()
					this_split=line.split(":")
					if len(this_split)==2:
						if get_headline and headline=="":
							headline+=this_split[0]
							first_line+=this_split[1]

						elif get_headline:
							headline+=","+this_split[0]	
							first_line+=","+this_split[1]
						
						if line_started:							
							this_csv+=this_split[1]
							line_started=False
						else:
							this_csv+=","+this_split[1]
					elif line.startswith("---------------------------") or line.startswith("split"):
						if get_headline and headline!="":
							this_csv=headline+"\n"+first_line+"\n"
							get_headline=False
							line_started=True	
						else:
							this_csv+="\n"
							line_started=True	
							
			f.close()
		except IsADirectoryError:
			pass	

out_file="big-table_"+path.replace("/", "")+".csv"			
f_out=open(out_file, "w+")				
f_out.write(this_csv)
f_out.close()
print(this_csv)
