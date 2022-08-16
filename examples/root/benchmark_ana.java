//#program

String file = "infile.hipo";

int iter = 4;

for(int j = 0; j < iter; j++){
HipoReader r = new HipoReader(file);
Bank p = r.getBank("REC::Particle");

Event e = new Event();
H1F   h = new H1F("h",120,0.0,10.0);
double px,py,pz,vx,vy,vz,vt;

BenchmarkTimer t = new BenchmarkTimer();
t.resume();
while(r.hasNext()){
    r.nextEvent(e);
    e.read(p);
    int rows = p.getRows();
    for(int row = 0; row < rows; row++){
      px = p.getFloat(1,row);
      py = p.getFloat(2,row);
      pz = p.getFloat(3,row);
      vx = p.getFloat(4,row);
      vy = p.getFloat(5,row);
      vz = p.getFloat(6,row);
      vt = p.getFloat(7,row);
      double value =  Math.sqrt(px*px + py*py + pz*pz)*Math.sqrt(vx*vx+vy*vy+vz*vz)
	 + vt + p.getInt(0,row)*p.getFloat(9,row)*p.getFloat(10,row)+p.getInt(11,row) - p.getInt(8,row);
       h.fill(value);
    }
}
t.pause();

System.out.println(t);
}

