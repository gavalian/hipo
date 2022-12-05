//#program
import twig.data.*;
import twig.math.*;
import twig.tree.*;
import twig.studio.*;
import twig.widgets.*;
import twig.widgets.LatexText.TextRotate;
import twig.widgets.LatexText.TextAlign;
import twig.graphics.*;
import twig.config.*;
import java.util.ArrayList;
import java.util.List;
//--------------------------------------
// Utils import
//--------------------------------------
import j4np.utils.*;
import j4np.utils.base.*;
//--------------------------------------
// Imports for Physics libraries
//--------------------------------------
import j4np.hipo5.data.*;
import j4np.hipo5.io.*;

public class Benchmark {

    public static void writerBenchmark(String file){

	HipoReader r = new HipoReader(file);
	Bank   p = r.getBank("REC::Particle");
	Event  e = new Event();
	HipoWriter w = new HipoWriter();

	w.getSchemaFactory().copy(r.getSchemaFactory());
	w.open(file+"_java.h5");
	
	BenchmarkTimer t = new BenchmarkTimer();

	Event eo = new Event();
	while(r.hasNext()){
	    r.nextEvent(e);
	    e.read(p);
	    eo.reset();
	    if(p.getRows()>0){
		t.resume();
		eo.write(p);
		w.addEvent(eo);
		t.pause();
	    }
	}
	
	w.close();
	System.out.println(t);
	System.out.printf("[FIN] (Java) hipo write : %10.4f sec\n",t.getSeconds());
    }
    
	public double calculate(Bank p, H1F h){
		int nrows = p.getRows();
		double value = 0;;
		for(int row = 0; row < nrows; row++){
		double px = p.getFloat(1,row);
		double py = p.getFloat(2,row);
		double pz = p.getFloat(3,row);
		double vx = p.getFloat(4,row);
		double vy = p.getFloat(5,row);
		double vz = p.getFloat(6,row);
		double vt = p.getFloat(7,row);
		//double value =  Math.sqrt(px*px + py*py + pz*pz)*Math.sqrt(vx*vx+vy*vy+vz*vz)
		 value =  (px*px + py*py + pz*pz)*(vx*vx+vy*vy+vz*vz)		
		    	 + vt + p.getInt(0,row)*p.getFloat(9,row)*p.getFloat(10,row)
				 +p.getInt(11,row) - p.getInt(8,row);
				 h.fill(value);
		}
		return value;
	}
	public double calculateRow(Bank p, int row){
		double value = 0;;
		/*double px = p.getFloat(1,row);
		double py = p.getFloat(2,row);
		double pz = p.getFloat(3,row);
		double vx = p.getFloat(4,row);
		double vy = p.getFloat(5,row);
		double vz = p.getFloat(6,row);
		double vt = p.getFloat(7,row);
*/
		value =  Math.sqrt(p.getFloat(1,row)*p.getFloat(1,row) 
		+ p.getFloat(2,row)*p.getFloat(2,row) + 
		p.getFloat(3,row)*p.getFloat(3,row))*
		Math.sqrt(
			p.getFloat(4,row)*p.getFloat(4,row)
			+ p.getFloat(5,row)*p.getFloat(5,row) + p.getFloat(6,row)*p.getFloat(6,row))
		    	 + p.getFloat(7,row) + p.getInt(0,row)*p.getFloat(9,row)*p.getFloat(10,row)
				 +p.getInt(11,row) - p.getInt(8,row);
		//double value =  Math.sqrt(px*px + py*py + pz*pz)*Math.sqrt(vx*vx+vy*vy+vz*vz)
		 /*value =  Math.sqrt(px*px + py*py + pz*pz)*Math.sqrt(vx*vx+vy*vy+vz*vz)		
		    	 + vt + p.getInt(0,row)*p.getFloat(9,row)*p.getFloat(10,row)
				 +p.getInt(11,row) - p.getInt(8,row);*/

		return value;
	}

	public double readBenchmark(String file){
		HipoReader r = new HipoReader(file);
		Bank p = r.getBank("REC::Particle");
		
		Event e = new Event();
		H1F   h = new H1F("h",120,0.0,10.0);
		double px,py,pz,vx,vy,vz,vt;
		
		BenchmarkTimer t = new BenchmarkTimer();
		BenchmarkTimer op = new BenchmarkTimer();
		t.resume();

		while(r.hasNext()){
			//r.next();
			r.nextEvent(e);
			e.read(p);
			//op.resume();
			//calculate(p,h);
			int rows = p.getRows();
			double value = 0;
			for(int row = 0; row < rows; row++){
			  value = calculateRow(p,row);
			  //System.out.println(calculateRow(p,row));
			  h.fill(value);
			}
			//h.fill(value);
			//op.pause();
		}
		t.pause();
	
	System.out.println(t);
	System.out.println(op);
	System.out.printf("\nexecution time : %10.4f sec\n",t.getSeconds());
	return t.getSeconds();
	}



    public static double benchmark(String file){

	HipoReader r = new HipoReader(file);
	Bank p = r.getBank("REC::Particle");
	
	Event e = new Event();
	H1F   h = new H1F("h",120,0.0,10.0);
	double px,py,pz,vx,vy,vz,vt;
	
	BenchmarkTimer t = new BenchmarkTimer();
	BenchmarkTimer op = new BenchmarkTimer();
	t.resume();

	while(r.hasNext()){
	    //r.next();
	    r.nextEvent(e);
	    e.read(p);
	    //op.resume();
	    int rows = p.getRows();
	        for(int row = 0; row < rows; row++){
	    px = p.getFloat(1,row);
		py = p.getFloat(2,row);
		pz = p.getFloat(3,row);
		vx = p.getFloat(4,row);
		vy = p.getFloat(5,row);
		vz = p.getFloat(6,row);
		vt = p.getFloat(7,row);
		//double value =  Math.sqrt(px*px + py*py + pz*pz)*Math.sqrt(vx*vx+vy*vy+vz*vz)
		double value =  (px*px + py*py + pz*pz)*(vx*vx+vy*vy+vz*vz)
		     + vt + p.getInt(0,row)*p.getFloat(9,row)*p.getFloat(10,row)+p.getInt(11,row) - p.getInt(8,row);
		h.fill(value);
		}
		//op.pause();
	}
	t.pause();
	
	System.out.println(t);
	System.out.println(op);
	return t.getSeconds();
    }


    public static double average(List<Double> list){
	double summ = 0.0;
	for(Double v : list) summ += v;
	return summ/list.size();
    }
    
    public static void main(String[] args){
	int iter = 5;
	String filename = args[0];
	Benchmark b = new Benchmark();
	b.writerBenchmark(filename);

	String rfile = filename + "_java.h5";

	double realTime = b.readBenchmark(rfile);
	b.readBenchmark(rfile);

	b.readBenchmark(rfile);

	b.readBenchmark(rfile);
	List<Double> times = new ArrayList<>();
	
	for(int j = 0; j < iter; j++){
	    //Benchmark.benchmark(args[0]);
		//Benchmark b = new Benchmark();
		double v = Benchmark.benchmark(rfile);
		times.add(v);
	}

	System.out.printf("[FIN] (Java) hipo read  : %10.4f\n",realTime);
    }
}

