# Sorting Files on SD card for SD2IEC

## The problem

Files on the SD card appear unsorted or sorted in a strange and unpredictable way when listed with the SD2IEC on the C64.
The same thing may happen on any other device too typically devices with limited ressources, shuch as MP3 players.
Also on modern TVs when you run a dia show and the pictures appear in strange orders.

## The background

While a modern OS like windows/unix sorts files always in some way (by name, date, size,..) there is a order of files they have on the raw file system.
This order depends on in which order the files were created in that folder. This might be sorted in some way but this is not guarateed.
Imagine you create the files a,b,c in bash like this:
    touch b  
    touch a  
    touch c  
If you now type ls -l you get
	a  
	b  
	c  
because ls sort files implicitly by name. But if you say ls -lU you get
	b  
	a  
	c  
because the -U switch disables any sorting and displays files as they are really stored on the file system.
When you now delete a and add x and type ls -lU you'll get
	b
	z
	c
because a occupied the second directory entry which got freed and is now taken by z.

On windows i don't know an easy way to show the real order of files, but the problem is the same there.

When you now copy a folder with files a,b,c in the order b,a,c any regular copy program (like windows explorer or unix cp) will process files in the file system order and replicate them in the same order on the destination.
This means, once file order is - for some reason - unsorted, this cannot be fixed by just copying files to another drive.

## The solution

On the C64 one might think about using a file browser with sorting capabilities. But this not a satisfactory solution because 1. files remain unsorted outside this browser and 2. sorting occupies CPU and storage ressources.

A better solution is to have them already in the desired order on the file system.  
On linux a sorted copying including all sub folders can be done by

    rsync -r source_dir/* destination_dir  

