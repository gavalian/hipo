import java.awt.Font;

public class Results{

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

	hroot.attr().setLegend("ROOT write, C++");
        hhipo.attr().setLegend("HIPO write, C++");
        hjava.attr().setLegend("HIPO write, Java");
        hroot6.attr().setLegend("ROOT6 Writer <VEC>, C++");
	
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

	TGCanvas c = new TGCanvas(800,400);
        //c.draw(hroot).draw(hhipo,"same").draw(hjava,"same");
        c.draw(hroot).draw(hhipo,"same").draw(hjava,"same");
        c.view().region().showLegend(0.05,0.95);
        c.view().region().axisX().getAttributes().setAxisTicksPosition(Arrays.asList(2.0,6.0,10.0,14.0));
        c.view().region().axisX().getAttributes().setAxisTicksString(Arrays.asList("MacBook(M1)","AMD","Intel/Xeon","MacBook(i9)"));
	c.view().region().setAxisLabelFont(new Font("Palatino",Font.PLAIN,20));
	c.view().region().setAxisTitleFont(new Font("Palatino",Font.PLAIN,20));

    }
    

	public static void write(){
		
		H1F hroot  = new H1F("hroot",21,-0.5,20.5);
		H1F hroot6 = new H1F("hroot6",21,-0.5,20.5);
		H1F hhipo  = new H1F("hhipo",21,-0.5,20.5);
		H1F hjava  = new H1F("hjava",21,-0.5,20.5);

		hroot.attr().set("fc=36");
		hroot6.attr().set("fc=57");
		hhipo.attr().set("fc=10");
		hjava.attr().set("fc=12");
	
	
		hroot.attr().setLegend("ROOT Writer, C++");
			hhipo.attr().setLegend("HIPO Writer, C++");
			hjava.attr().setLegend("HIPO Writer, Java");
			hroot6.attr().setLegend("ROOT6 Writer <VEC>, C++");
		
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
	
		TGCanvas c = new TGCanvas(800,420);
			//c.draw(hroot).draw(hhipo,"same").draw(hjava,"same");
		c.draw(hroot).draw(hroot6,"same").draw(hhipo,"same").draw(hjava,"same");
		c.view().region().showLegend(0.65,0.95);
		c.view().region().axisX().getAttributes().setAxisTicksPosition(Arrays.asList(2.5,7.5,12.5,17.5));
		c.view().region().axisX().getAttributes().setAxisTicksString(Arrays.asList("MacBook M1","iFarm AMD","Clara Xeon","MacBook i9"));	
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
	
	hroot.attr().setLegend("ROOT read, C++");
	hhipo.attr().setLegend("HIPO read, C++");
	hjava.attr().setLegend("HIPO read, Java");
	hroot6.attr().setLegend("ROOT6 <VEC>, C++");
	
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
}

public static void draw6(){

	H1F hroot  = new H1F("hroot",21,-0.5,20.5);
	H1F hroot6 = new H1F("hroot6",21,-0.5,20.5);
	H1F hhipo  = new H1F("hhipo",21,-0.5,20.5);
	H1F hjava  = new H1F("hjava",21,-0.5,20.5);
	
	hroot.attr().set("fc=66,lc=0");
	hroot6.attr().set("fc=57,lc=0");
	hhipo.attr().set("fc=10,lc=0");
	hjava.attr().set("fc=12,lc=0");
	
	
	hroot.attr().setLegend("ROOT, C++");
	hhipo.attr().setLegend("HIPO, C++");
	hjava.attr().setLegend("HIPO, Java");
	hroot6.attr().setLegend("ROOT6 <VEC>, C++");
	
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
	c.view().region().axisX().getAttributes().setAxisTicksString(Arrays.asList("MacBook M1","iFarm AMD","Clara Xeon","MacBook i9"));
}

    
public static void main(String[] args){
    Results.draw7();
}

}
