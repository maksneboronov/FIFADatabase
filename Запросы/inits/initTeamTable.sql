USE fifaDataBase;

--IF OBJECT_ID('dbo.Teams', 'U') IS NOT NULL  
--	DROP TABLE dbo.Teams; 
	
--CREATE TABLE dbo.Teams 
--(  
--	tid		INT				NOT NULL IDENTITY(0, 1),  
--	tname	NVARCHAR(30)	NOT NULL,  	
--	tcoach	NVARCHAR(50)	NOT NULL,  
--	tgroup  CHAR			NOT NULL, 
--	tgame	INT				NOT NULL DEFAULT(0),   
--	twon	INT				NOT NULL DEFAULT(0),   
--	tdrawn	INT				NOT NULL DEFAULT(0),   
--	tlost	INT				NOT NULL DEFAULT(0),   
--	tgf		INT				NOT NULL DEFAULT(0),   
--	tga		INT				NOT NULL DEFAULT(0),  
--	tgd		INT				NOT NULL DEFAULT(0),   
--	tpoints INT				NOT NULL DEFAULT(0), 
	   
--	CONSTRAINT teamsUniqueTeamName UNIQUE(tname), 
--	CONSTRAINT teamsPrimaryKeyTid PRIMARY KEY(tid) 
--);

--ALTER TABLE dbo.Orders  
--	ADD CONSTRAINT FK_Orders_Employees  
--	FOREIGN KEY(empid)				--�������� empid ��� ������� ����
--	REFERENCES dbo.Employees(empid)	--������� ��������� �� ���������
--									--���� ������ �������������� �������� - ������

--INSERT INTO dbo.Teams(tname, tcoach, tgroup) VALUES ('������', '�������� �.�.', '�');
--INSERT INTO dbo.Teams(tname, tcoach, tgroup) VALUES ('������', '���-��', '�');
INSERT INTO dbo.Teams(tname, tcoach, tgroup) VALUES ('��������', '���-��', 'C');
INSERT INTO dbo.Teams(tname, tcoach, tgroup) VALUES ('���������', '���-��', 'D');
INSERT INTO dbo.Teams(tname, tcoach, tgroup) VALUES ('���', '���-��', 'C');