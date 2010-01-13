import java.util.*;
import java.io.*;

public class Size
{
	int ram, ssd, ramtmp, ssdtmp;							//ram and ssd size; ram and ssd free space
	Vector<FileType> files;
	Vector<Integer> profile;
	int i, n, index, size, delindex, delsize;
	Vector <Integer> ramfiles = new Vector<Integer>();
	Vector <Integer> ssdfiles = new Vector<Integer>();
	double time = 0.0, timetmp = 0.0;
	PrintStream ps;
	Times times;
	int maxsize;
	
	public Size(int r, int s, Vector<FileType> f, Vector<Integer> p, Times t, int m)
	{
		ram = r;
		ssd = s;
		files = f;
		profile = p;
		times = t;
		maxsize = m;
	}
	
	void insert() throws IOException
	{
		
		ramtmp = ram;
		ssdtmp = ssd;
		n = profile.size();
		ps = new PrintStream( new FileOutputStream("Results2.txt"));
		
		for (i=0;i<n;i++)
		{
			index = profile.elementAt(i).intValue();
			size = files.elementAt(index).size;
			
			if (ramfiles.contains(index))	 					//the file is already in ram
			{
				timetmp = times.ram_acc + size/times.ram_rate;
				ps.print(index+" ");
				ps.print(size+" ");
				ps.println(timetmp+" ");
				time += timetmp;
			}
			else if (ssdfiles.contains(index)) 					//the file is already in ssd
			{
				timetmp = times.ssd_acc + size/times.ssd_rate;
				ps.print(index+" ");
				ps.print(size+" ");
				ps.println(timetmp+" ");
				time += timetmp;
			}
			else											//the file will be copied in ram or ssd (if it fits)
			{
				if (size<=maxsize)							//file to be placed in ram
				{
					if (size < ramtmp) 						//we have space in ram
					{ 
						ramtmp-=size;
						ramfiles.addElement(index);
						timetmp = times.hdd_acc + size/times.hdd_rate + times.ram_copy*size;
						ps.print(index+" ");
						ps.print(size+" ");
						ps.println(timetmp+" ");
						time += timetmp;
					}
					else if (size<ram)						//the file can fit in ram
					{
						while (ramtmp<size)					//we remove files from ram to make room for the new file
						{
							delindex = ramfiles.elementAt(0).intValue();
							delsize = files.elementAt(delindex).size;
							ramfiles.removeElementAt(0);
							ramtmp += delsize;
						}
						
						ramtmp-=size;						//we insert the new file in ram
						ramfiles.addElement(index);
						timetmp = times.hdd_acc + size/times.hdd_rate + times.ram_copy*size;
						ps.print(index+" ");
						ps.print(size+" ");
						ps.println(timetmp+" ");
						time += timetmp;
					}
					else 									//the file can't fit in ram and is accessed from hdd
					{
						timetmp = times.hdd_acc + size/times.hdd_rate;
						ps.print(index+" ");
						ps.print(size+" ");
						ps.println(timetmp+" ");
						time += timetmp;
					}	
				}
				
				else										//file to be placed in ssd 
				{
					if (size < ssdtmp) 						//we have space in ssd
					{ 
						ssdtmp-=size;
						ssdfiles.addElement(index);
						timetmp = times.hdd_acc + size/times.hdd_rate + times.ssd_copy*size;
						ps.print(index+" ");
						ps.print(size+" ");
						ps.println(timetmp+" ");
						time += timetmp;
					} 
					
					else if (size<ssd)						//the file can fit in ssd
					{
						while (ssdtmp<size)					//we remove files from ssd to make room for the new file
						{
							delindex = ssdfiles.elementAt(0).intValue();
							delsize = files.elementAt(delindex).size;
							ssdfiles.removeElementAt(0);
							ssdtmp += delsize;
						}
						
						ssdtmp-=size;						//we insert the new file in ssd
						ssdfiles.addElement(index);
						timetmp = times.hdd_acc + size/times.hdd_rate + times.ssd_copy*size;
						ps.print(index+" ");
						ps.print(size+" ");
						ps.println(timetmp+" ");
						time += timetmp;
					}
					else									//the file can't fit in ssd and is accessed from hdd	
					{
						timetmp = times.hdd_acc + size/times.hdd_rate;
						ps.print(index+" ");
						ps.print(size+" ");
						ps.println(timetmp+" ");
						time += timetmp;
					}
					
				}
							
			}
						
		}
		ps.print(time+" ");
		ps.close();
	}
}
