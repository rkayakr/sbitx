# sbitx
adding tool for creating ADIF format log file

Log2ADIF.py is a simple python program that reads the sbitx log database and creates a log file in ADIF format.
It expects to find the sbitx.db file in /home/pi/sbitx/data and leaves the logADIF.adif in that directory.
Place log2ADIF.py in /home/pi/sbitx and make sure it is executable. If it is not, from a terminal in /home/pi/sbitx type
chmod +x Log2ADIF.py

To execute from a terminal in /home/pi/sbitx type
./Log2ADIF.py

If /home/pi/sbitx has been added to PATH you can execute by typing Log2ADIF.py from any terminal.

  Bob Benedict, KD8CGH, June 2023
