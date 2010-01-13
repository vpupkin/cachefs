import java.io.*; 
import java.util.*;


class FileType												//stores a file: index and size
{
	int index;
	int size;
	
	public FileType(int i, int s)
	{
		index = i;
		size = s;
	}
	
}


class Times
{
	int ram_acc, ssd_acc, hdd_acc;						//ram, ssd and hdd acces time
	int ram_rate, ssd_rate, hdd_rate;					//ram, ssd and hdd read rate
	int ram_copy, ssd_copy; 							//time to copy from hdd to ram, respectively ssd
}

class Main
{
	int ram, ssd;										//ram and ssdsize
	Vector<FileType> files = new	Vector<FileType>();	//vector of files
	Vector<Integer> profile = new Vector<Integer>();    //the order in which files are accesed
	Times times = new Times();							
	int maxsize;										//maximum size of files to be copied in ram
	
	public void readdata() throws IOException
	{
		RandomAccessFile f1, f2, f3;
		f1 = new RandomAccessFile("Files.txt","r");
		f2 = new RandomAccessFile("Profile.txt","r");
		f3 = new RandomAccessFile("Times.txt","r");
		String line = null;
		StringTokenizer st;
		FileType file;
					
		line = f1.readLine();
		while (line!=null)
		{
			st = new StringTokenizer(line," \n");
			file = new FileType(-1,0);
			file.index = Integer.parseInt(st.nextToken());	//pairs of index and size of a file	
			file.size  = Integer.parseInt(st.nextToken());
			files.addElement(file);
			line = f1.readLine();
		}
		f1.close();
		
		line = f2.readLine();
		ram = Integer.parseInt(line);						//ram size
		line = f2.readLine();
		ssd = Integer.parseInt(line);						//ssd size
		line = f2.readLine();		
		maxsize = Integer.parseInt(line);					//maximum size of files to be copied in ram 
		line = f2.readLine();
		st = new StringTokenizer(line," \n");
		while(st.hasMoreTokens())
		{
			profile.addElement(new Integer(st.nextToken()));//the order in which files are accesed
		}
		f2.close();
		
		line = f3.readLine();
		st = new StringTokenizer(line," \n");
		times.ram_acc = Integer.parseInt(st.nextToken());	//acces times
		times.ssd_acc = Integer.parseInt(st.nextToken());
		times.hdd_acc = Integer.parseInt(st.nextToken());
		line = f3.readLine();
		st = new StringTokenizer(line," \n");
		times.ram_rate = Integer.parseInt(st.nextToken());	//read rates 
		times.ssd_rate = Integer.parseInt(st.nextToken());
		times.hdd_rate = Integer.parseInt(st.nextToken());
		line = f3.readLine();
		st = new StringTokenizer(line," \n");
		times.ram_copy = Integer.parseInt(st.nextToken());	//copy times
		times.ssd_copy = Integer.parseInt(st.nextToken());
		f3.close();
				
	}
		
}


class Test
{
	public static void main(String args[]) throws IOException{
		
		Main m = new Main();
		m.readdata();
		Simple simple = new Simple(m.ram, m.ssd, m.files, m.profile, m.times);
		simple.insert();
		Size size = new Size(m.ram, m.ssd, m.files, m.profile, m.times, m.maxsize);
		size.insert();
	}
}