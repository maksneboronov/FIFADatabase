USE fifaDataBase;

IF OBJECT_ID('dbo.WorkingPeople', 'U') IS NOT NULL  
	DROP TABLE dbo.WorkingPeople; 
	
CREATE TABLE dbo.WorkingPeople 
(  
	wid					INT				NOT NULL IDENTITY(0, 1),  
	mid					INT				NOT NULL,  	
	pid					INT				NOT NULL,   
	   	
	CONSTRAINT wpPrimaryKeyWid PRIMARY KEY(wid),
	CONSTRAINT wpForeignKeyPid FOREIGN KEY(pid) REFERENCES dbo.Persons(pid) ON DELETE CASCADE ON UPDATE CASCADE,
	CONSTRAINT wpForeignKeyMid FOREIGN KEY(mid) REFERENCES dbo.Matches(mid) ON DELETE CASCADE ON UPDATE CASCADE
);

