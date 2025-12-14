
# python dow.py >dow_py.txt

# https://www.geeksforgeeks.org/python/weekday-function-of-datetime-date-class-in-python/
# weekday()  "Monday is 0 and Sunday is 6"

from datetime import date, timedelta

d = date(1900, 1, 1)
while True:
	print(d, (d.weekday()+1)%7 )
	if d == date(2155, 12, 31):
		break
	d = d + timedelta(days=1)
