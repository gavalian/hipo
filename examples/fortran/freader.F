      PROGRAM HIPOREADER
      INTEGER STATUS
      INTEGER COUNTER
      INTEGER NROWS,NR
      REAL     PX(50)
      INTEGER  PID(50)
      INTEGER  CHARGE(50)
      CALL HIPO_FILE_OPEN('infile.hipo')

      STATUS = 0
      COUNTER = 0

      DO WHILE (STATUS.EQ.0)
         CALL HIPO_FILE_NEXT(STATUS)
         CALL HIPO_READ_BANK( 'REC::Particle',NROWS)
         CALL HIPO_READ_INT(  'REC::Particle','pid',NR,PID,50)
         CALL HIPO_READ_INT(  'REC::Particle','charge',NR,CHARGE,50)
         CALL HIPO_READ_FLOAT('REC::Particle','px', NR, PX,50)
         WRITE(*,*) '--- event # ', COUNTER, ' ----------'
         WRITE(*,*) (PID(j),J=1,NROWS)
         WRITE(*,*) (PX( j),J=1,NROWS)
         WRITE(*,*) (CHARGE( j),J=1,NROWS)

         COUNTER = COUNTER + 1
      END DO
      write(*,*)'# EVENTS PROCESSED = ',COUNTER
      END
