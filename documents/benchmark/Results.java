import java.awt.Font;


public class Results{

    public static int X_SIZE = 900;
    public static int Y_SIZE = 550;
    
	public static void readBench(){
	    TStyle.getInstance().getPalette().setColorScheme("gold10");
		TStyle.getInstance().setDefaultPaveTextFont( new Font("Paletino",Font.PLAIN,18));
			TStyle.getInstance().setDefaultAxisLabelFont(new Font("Paletino",Font.PLAIN,18));
			TStyle.getInstance().setDefaultAxisTitleFont(new Font("Paletino",Font.PLAIN,20));
	
		BarChartBuilder b = new BarChartBuilder();
		DataGroup group =
		    b.addEntry("ROOT TTree read, C++",3.9121,5.17,5.17)
		    .addEntry("ROOT RNTuple read, C++",8.92,9.31,10.8)
		    .addEntry("HIPO Lib read, C++", 2.33,3.15,3.23)
		    .addEntry("HIPO Read, Java", 3.75,5.59,5.10)
		    
		    //b.addEntry("ROOT TTree read, C++",4.0650,5.2855,5.5584)
		    //               .addEntry("ROOT RNTuple read, C++",0,9.31,10.8)
		    //.addEntry("HIPO clas12 read, C++", 0,6.14,5.37)
		    //.addEntry("HIPO Lib read, C++", 2.5788,4.0313,3.7532)
		    //.addEntry("HIPO Lib read, Java",3.8582,6.4356,6.9700)
			//.addEntry("ROOT RNTuple, C++", 3.2,1.2,2.45)
			//.addEntry("HIPO Read, C++", 3.2,1.2,2.45)
			//.addEntry("HIPO Read, Java", 3.7,6.2,1.6,2)
				
			.setTitleY("Time (sec)")
			.setColors(new int[]{52,72,29,49,79})
			.setLabels(new String[]{"Apple M1","Intel","AMD"})
			.build();
			
			System.out.println("size = " + group.getData().size());
			TGCanvas c = new TGCanvas(Results.X_SIZE, Results.Y_SIZE);
			//for(DataSet ds : group.getData()) c.draw(ds, "same");
			c.view().region().draw(group);//.showLegend(0.05, 0.95);
			c.view().region().showLegend(0.05, 0.95);
			c.view().export("bench_root_vs_hipo_read.pdf","pdf");
		}

		public static void readBenchPartial(){
			TStyle.getInstance().getPalette().setColorScheme("gold10");
			TStyle.getInstance().setDefaultPaveTextFont( new Font("Paletino",Font.PLAIN,18));
				TStyle.getInstance().setDefaultAxisLabelFont(new Font("Paletino",Font.PLAIN,18));
				TStyle.getInstance().setDefaultAxisTitleFont(new Font("Paletino",Font.PLAIN,20));
		
			BarChartBuilder b = new BarChartBuilder();
			DataGroup group =
				b.addEntry("ROOT TTree read, C++",1.17363,1.584,1.6027)
				//.addEntry("ROOT RNTuple read, C++",8.92,9.31,10.8)
				.addEntry("HIPO Lib read, C++", 0.6757,0.9764,0.9683)
				.addEntry("HIPO Read, Java", 1.0875,1.6769,1.581)
				
				//b.addEntry("ROOT TTree read, C++",4.0650,5.2855,5.5584)
				//               .addEntry("ROOT RNTuple read, C++",0,9.31,10.8)
				//.addEntry("HIPO clas12 read, C++", 0,6.14,5.37)
				//.addEntry("HIPO Lib read, C++", 2.5788,4.0313,3.7532)
				//.addEntry("HIPO Lib read, Java",3.8582,6.4356,6.9700)
				//.addEntry("ROOT RNTuple, C++", 3.2,1.2,2.45)
				//.addEntry("HIPO Read, C++", 3.2,1.2,2.45)
				//.addEntry("HIPO Read, Java", 3.7,6.2,1.6,2)
					
				.setTitleY("Time (sec)")
			    .setColors(new int[]{52,49,79})
				.setLabels(new String[]{"Apple M1","Intel","AMD"})
				.build();
				
				System.out.println("size = " + group.getData().size());
				TGCanvas c = new TGCanvas(Results.X_SIZE, Results.Y_SIZE);
				//for(DataSet ds : group.getData()) c.draw(ds, "same");
				c.view().region().draw(group);//.showLegend(0.05, 0.95);
				c.view().region().showLegend(0.05, 0.95);
				c.view().export("bench_root_vs_hipo_read_partial.pdf","pdf");
			}

    public static void writeBench(){

	TStyle.getInstance().getPalette().setColorScheme("gold10");
	TStyle.getInstance().setDefaultPaveTextFont( new Font("Paletino",Font.PLAIN,18));
        TStyle.getInstance().setDefaultAxisLabelFont(new Font("Paletino",Font.PLAIN,18));
        TStyle.getInstance().setDefaultAxisTitleFont(new Font("Paletino",Font.PLAIN,20));

	BarChartBuilder b = new BarChartBuilder();
	DataGroup group =
	    b.addEntry("ROOT TTree write, C++",47.714,71.40,64.52)
	     .addEntry("ROOT RNtuple write, C++",58.12,64.00,57.0)
	     .addEntry("HIPO Lib write (c++)",6.1,7.641,7.33)
	    .addEntry("HIPO Lib write, Java",5.0097,7.8681, 7.2520)
	    //b.addEntry("ROOT TTree write, C++",47.28,65.57,79.75)
	                  
	    //.addEntry("ROOT RNtuple write, C++",0.0,64.00,57.0)
            // .addEntry("HIPO clas12 write, C++", 0.0,16.63,16.58)
	    //               .addEntry("HIPO Lib write, C++", 6.01,7.74,8.74)
	    //.addEntry("HIPO Lib write, Java",5.06,7.39,9.47)
	    //.addEntry("ROOT RNTuple, C++", 3.2,1.2,2.45)
	    //.addEntry("HIPO Read, C++", 3.2,1.2,2.45)
	    //.addEntry("HIPO Read, Java", 3.7,6.2,1.6,2)
            
	    .setTitleY("Time (sec)")
	    .setColors(new int[]{52,72,29,49,79})
	    .setLabels(new String[]{"Apple M1","Intel","AMD"})
	    .build();
        
        System.out.println("size = " + group.getData().size());
        TGCanvas c = new TGCanvas(Results.X_SIZE, Results.Y_SIZE);
        //for(DataSet ds : group.getData()) c.draw(ds, "same");
        c.view().region().draw(group);//.showLegend(0.05, 0.95);
        c.view().region().showLegend(0.05, 0.95);
		c.view().export("bench_root_vs_hipo_write.pdf","pdf");
    }
    
    public static void write7(){

	H1F hroot  = new H1F("hroot",17,-0.5,16.5);
	H1F hroot6 = new H1F("hroot6",17,-0.5,16.5);
	H1F hhipo  = new H1F("hhipo",17,-0.5,16.5);
	H1F hjava  = new H1F("hjava",17,-0.5,16.5);
	
	hroot.attr().set("fc=76");
        hroot6.attr().set("fc=57");
        hhipo.attr().set("fc=10");
        hjava.attr().set("fc=12");

	hroot.attr().setTitleY("Time [sec]");

	hroot.attr().setLegend("RNTuple Write, C++");
	hhipo.attr().setLegend("HIPO Write, C++");
	hjava.attr().setLegend("HIPO Write, Java");
	hroot6.attr().setLegend("TTree <VEC> Write, C++");
	
	// ---> Macbook M1
	hroot.setBinContent(1 , 43.7);
	hroot6.setBinContent(1 , 12.6);
	hhipo.setBinContent(2, 6.2);
	hjava.setBinContent(3, 5.15);
	// CLARA Xeon 2.60 GHz
	hroot.setBinContent(5 , 65.0);
        hroot6.setBinContent(5 , 21.11);
	hhipo.setBinContent(6, 8.26);
	hjava.setBinContent(7,8.7);
	// iFarm AMD
        hroot.setBinContent(9 , 59.8);
        hroot6.setBinContent(9 , 24.25);
        hhipo.setBinContent(10, 8.25);
	hjava.setBinContent(11, 7.55);
	// Macbook i9
	hroot.setBinContent(13 , 55.43);
        hroot6.setBinContent(13 , 17.95);
        hhipo.setBinContent(14, 9.13);
        hjava.setBinContent(15, 7.35);
	TGCanvas c = new TGCanvas(800,400);
        //c.draw(hroot).draw(hhipo,"same").draw(hjava,"same");
        c.draw(hroot).draw(hhipo,"same").draw(hjava,"same");
        c.view().region().showLegend(0.05,0.95);
        c.view().region().axisX().getAttributes().setAxisTicksPosition(Arrays.asList(2.0,6.0,10.0,14.0));
        c.view().region().axisX().getAttributes().setAxisTicksString(Arrays.asList("MacBook(M1)","AMD","Intel/Xeon","MacBook(i9)"));
	c.view().region().setAxisLabelFont(new Font("Palatino",Font.PLAIN,20));
	c.view().region().setAxisTitleFont(new Font("Palatino",Font.PLAIN,20));

	c.view().export("root7_vs_hipo_write.pdf","pdf");
    }
    

	public static void write(){
		
		H1F hroot  = new H1F("hroot",21,-0.5,20.5);
		H1F hroot6 = new H1F("hroot6",21,-0.5,20.5);
		H1F hhipo  = new H1F("hhipo",21,-0.5,20.5);
		H1F hjava  = new H1F("hjava",21,-0.5,20.5);

		hroot.attr().set("fc=76");
		hroot6.attr().set("fc=57");
		hhipo.attr().set("fc=10");
		hjava.attr().set("fc=12");
		hroot.attr().setTitleY("Time [sec]");
	
		hroot.attr().setLegend("RNTuple Write, C++");
		hhipo.attr().setLegend("HIPO Write, C++");
		hjava.attr().setLegend("HIPO Write, Java");
		hroot6.attr().setLegend("TTree <VEC> Write, C++");
		
		// ---> Macbook M1
		hroot.setBinContent(1 , 43.7);
		hroot6.setBinContent(2 , 12.6);
		hhipo.setBinContent(3, 6.2);
		hjava.setBinContent(4, 5.15);
		// CLARA Xeon 2.60 GHz
		hroot.setBinContent(6 , 65.0);
		hroot6.setBinContent(7 , 21.11);
		hhipo.setBinContent(8, 8.26);
		hjava.setBinContent(9,8.7);
		// iFarm AMD
		hroot.setBinContent(11 , 59.8);
		hroot6.setBinContent(12 , 24.25);
		hhipo.setBinContent(13, 8.25);
		hjava.setBinContent(14, 7.55);
	
		TGCanvas c = new TGCanvas(800,400);
			//c.draw(hroot).draw(hhipo,"same").draw(hjava,"same");
		c.draw(hroot).draw(hroot6,"same").draw(hhipo,"same").draw(hjava,"same");
		c.view().region().showLegend(0.65,0.95);
		c.view().region().axisX().getAttributes().setAxisTicksPosition(Arrays.asList(2.5,7.5,12.5,17.5));
		c.view().region().axisX().getAttributes().setAxisTicksString(Arrays.asList("MacBook(M1)","AMD","Intel/Xeon","MacBook(i9)"));
	c.view().region().setAxisLabelFont(new Font("Palatino",Font.PLAIN,20));
	c.view().region().setAxisTitleFont(new Font("Palatino",Font.PLAIN,22));
	c.repaint();
	c.view().export("root6_vs_hipo_write.pdf","pdf");

	}
		

    public static void draw7(){

	H1F hroot  = new H1F("hroot",17,-0.5,16.5);
	H1F hroot6 = new H1F("hroot6",17,-0.5,16.5);
	H1F hhipo  = new H1F("hhipo",17,-0.5,16.5);
	H1F hjava  = new H1F("hjava",17,-0.5,16.5);
	
	hroot.attr().set("fc=76");
	hroot6.attr().set("fc=7");
	hhipo.attr().set("fc=10");
	hjava.attr().set("fc=12");
	hroot.attr().setTitleY("Time [sec]");
	
	hroot.attr().setLegend("RNTuple, C++");
	hhipo.attr().setLegend("HIPO, C++");
	hjava.attr().setLegend("HIPO, Java");
	hroot6.attr().setLegend("TTree <VEC>, C++");
	
	// -- MacBook M1
	hroot6.setBinContent(4 ,10.6);
	hroot.setBinContent( 1 ,4.00);
	hhipo.setBinContent( 2 ,2.58);
	hjava.setBinContent( 3 ,2.79);
	// -- IFARM AMD
	hroot6.setBinContent(8 ,17.6);
	hroot.setBinContent(5,5.60);
	hhipo.setBinContent(6,3.90);
	hjava.setBinContent(7,5.1);
	// -- CLARA Xeon
	hroot6.setBinContent(12 ,16.8);
	hroot.setBinContent(9 ,9.8);
	hhipo.setBinContent(10,3.73);
	hjava.setBinContent(11,5.23);
	// -- MAC BOOK PRO
	hroot.setBinContent(13 , 8.46);
	hhipo.setBinContent(14 , 4.56);
	hjava.setBinContent(15 , 5.23);
	
	TGCanvas c = new TGCanvas(800,400);
	//c.draw(hroot).draw(hhipo,"same").draw(hjava,"same");
	c.draw(hroot).draw(hhipo,"same").draw(hjava,"same");
	c.view().region().showLegend(0.05,0.95);
	c.view().region().axisX().getAttributes().setAxisTicksPosition(Arrays.asList(2.0,6.0,10.0,14.0));
	//Arrays.asList("MacBook(M1)","AMD(EPYC-7502)","Intel/Xeon(CPU E5-2697A)","MacBook(i9)")
	c.view().region().axisX().getAttributes().setAxisTicksString(Arrays.asList("MacBook(M1)","AMD","Intel/Xeon","MacBook(i9)"));
	c.view().region().setAxisLabelFont(new Font("Palatino",Font.PLAIN,20));
	c.view().region().setAxisTitleFont(new Font("Palatino",Font.PLAIN,22));
	c.repaint();
	c.view().export("root7_vs_hipo_read.pdf","pdf");
}

public static void draw6(){

	H1F hroot  = new H1F("hroot",21,-0.5,20.5);
	H1F hroot6 = new H1F("hroot6",21,-0.5,20.5);
	H1F hhipo  = new H1F("hhipo",21,-0.5,20.5);
	H1F hjava  = new H1F("hjava",21,-0.5,20.5);
	
	hroot.attr().set("fc=76");
	hroot6.attr().set("fc=57");
	hhipo.attr().set("fc=10");
	hjava.attr().set("fc=12");
	hroot.attr().setTitleY("Time [sec]");
	
	hroot.attr().setLegend("RNTuple, C++");
	hhipo.attr().setLegend("HIPO, C++");
	hjava.attr().setLegend("HIPO, Java");
	hroot6.attr().setLegend("TTree <VEC>, C++");
	
	// -- MacBook M1

	hroot.setBinContent( 1 ,4.00);
	hroot6.setBinContent(2 ,10.6);
	hhipo.setBinContent( 3 ,2.58);
	hjava.setBinContent( 4 ,2.79);
	// -- IFARM AMD

	hroot.setBinContent(6,5.60);
	hroot6.setBinContent(7 ,17.6);
	hhipo.setBinContent(8,3.90);
	hjava.setBinContent(9,5.1);
	// -- CLARA Xeon
	
	hroot.setBinContent(11 ,9.8);
	hroot6.setBinContent(12 ,16.8);
	hhipo.setBinContent(13,3.73);
	hjava.setBinContent(14,5.23);

	// -- MAC BOOK PRO
	hroot.setBinContent(16 , 8.46);
	hroot6.setBinContent(17 ,12.9);
	hhipo.setBinContent(18 , 4.56);
	hjava.setBinContent(19 , 5.23);
	
	TGCanvas c = new TGCanvas(800,400);
	//c.draw(hroot).draw(hhipo,"same").draw(hjava,"same");
	c.draw(hroot).draw(hhipo,"same").draw(hjava,"same").draw(hroot6,"same");
	c.view().region().showLegend(0.05,0.95);
	c.view().region().axisX().getAttributes().setAxisTicksPosition(Arrays.asList(2.5,7.5,12.5,17.5));
	c.view().region().axisX().getAttributes().setAxisTicksString(Arrays.asList("MacBook(M1)","AMD","Intel/Xeon","MacBook(i9)"));
	c.view().region().setAxisLabelFont(new Font("Palatino",Font.PLAIN,20));
	c.view().region().setAxisTitleFont(new Font("Palatino",Font.PLAIN,22));
	c.repaint();
	c.view().export("root6_vs_hipo_read.pdf","pdf");
}

    
public static void main(String[] args){
    
    Results.draw7();
}

}
