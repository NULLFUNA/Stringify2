/*

 *----------------------*
 | Created by nullFuna 	|
 | github.com/nullFuna	|
 *----------------------*

*/
#include <QCoreApplication>
#include <QDir>
#include <QFile>

//	Result extension name
//	Before: shader.glsl
//	After:	shader.c | shader.cpp
constexpr const char*	RESULT_EXTNAME = ".c";

inline QString	RemoveSpaces( QString sLine ){
	QString sResult = "";
	QString	sTemp = "";
	bool	bStillEmpty = true;

	//	Strip trailing whitespace from line
	for( int i = sLine.size()-1; i >= 0 ; i-- )
		if( bStillEmpty )
			if(sLine[i] == ' ' || sLine[i] == '\t')
				continue;
			else {
				sTemp.append(sLine[i]);
				bStillEmpty = false;
			}
		else
			sTemp.append(sLine[i]);

	//	Temp string is flipped
	//	Well, flip it again
	for( int i = sTemp.size()-1; i >= 0 ; i-- )
		sResult.append(sTemp[i]);

	return sResult;
}

void	glsl2cpp( QString sGLSL ){

	QString	sShaderName = sGLSL.split('.')[0];
	QString	sResultFile = sShaderName + RESULT_EXTNAME;

	QFile	qInput(sGLSL);
	QFile	qOutput(sResultFile);

	if(!qInput.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	if(!qOutput.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)){
		qInput.close();
		return;
	}

	qOutput.write( ( "const char *fallbackShader_" + sShaderName + " =\n" ).toStdString().c_str() );

	QTextStream qReader(&qInput);

	while (!qReader.atEnd()) {
		QString sLine = qReader.readLine();
		qOutput.write( ( "\"" + RemoveSpaces(sLine) + "\\n\"\n" ).toStdString().c_str() );
	}


	qOutput.write(";\n");
	qInput.close();
	qOutput.close();

}

QStringList	GetFiles( const char* pchPath ) {
	QDir qDirectory( pchPath );
	return qDirectory.entryList(QStringList() << "*.glsl" ,QDir::Files);
}

int main(int argc, char *argv[])
{
	const char*	pchPath = "";

	//	Select directory from arguments if aviable
	//	Or use working directory
	if(argc == 2)
		pchPath = argv[1];

	//	Support for old stringify
	if(argc == 3){
		glsl2cpp(argv[1]);
		return 0;
	}

	QStringList lFiles = GetFiles(pchPath);

	//	Convert files
	foreach(QString sFilename, lFiles)
		glsl2cpp(sFilename);


	return 0;
}
