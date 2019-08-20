USE fifaDataBase;

IF OBJECT_ID('dbo.Stadium', 'U') IS NOT NULL  
	DROP TABLE dbo.Stadium; 
	
CREATE TABLE dbo.Stadium 
(  
	stid				INT				NOT NULL IDENTITY(0, 1),  
	pid					INT				NULL,
	sname				NVARCHAR(30)	NOT NULL,  	
	stown				NVARCHAR(30)	NOT NULL,  
	sseatcount			INT				NOT NULL DEFAULT(0),  
	tsectorcount		INT				NOT NULL DEFAULT(0),   
	tscheme				IMAGE			NULL DEFAULT(0x)
	   	
	CONSTRAINT stadiumForeignKeyPid FOREIGN KEY(pid) REFERENCES dbo.Persons(pid) ON DELETE SET NULL,
	CONSTRAINT stadiumPrimaryKeyStid PRIMARY KEY(stid) 
);

--INSERT INTO dbo.Stadium(sname, stown) VALUES ('Лужники1', 'Москва');
