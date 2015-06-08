//---------------------------------------------------------------------------
#include "precompiledHeaders.h"
//---------------------------------------------------------------------------
#include "MethodBrowser.h"
//---------------------------------------------------------------------------
#include "ClassMethods.h"
#include "ExceptionHandling.h"
#include "TimerMethodWidget.h"
//---------------------------------------------------------------------------


MetaClass* MethodBrowser::MethodBrowserMetaClass = NULL;

const MetaClass* MethodBrowser::getInstanceClass() const {
   return getClass();
}

Object* MethodBrowser::newInstance() {
   return new MethodBrowser;
}

const MetaClass* MethodBrowser::getClass() {		
   if (MethodBrowserMetaClass==NULL) {		
	MethodBrowserMetaClass = new MetaClass( "MethodBrowser" , "no description.",		
                                            newInstance, sizeof( MethodBrowser ));		
   }		
   return MethodBrowserMetaClass;		
}		
		
void MethodBrowser::init(const MetaClass *parentClass) {		
   ((MetaClass*)getClass())->init(parentClass, MetaClass::FIRST_VERSION + 0);		
   MethodBrowserMetaClass->copyParentProperties();		
}		
		
void MethodBrowser::shutdown() {		
   delete MethodBrowserMetaClass;		
}		

void MethodBrowser::update() {m_Widget->updateEditor();}

card32 MethodBrowser::getPreferredWidth() {return m_Widget->width();}
card32 MethodBrowser::getPreferredHeight()  {return m_Widget->height();}

QWidget * MethodBrowser::makeEditor( QWidget * parent, Object * obj ) { 
	m_Widget = new MethodBrowserWidget( parent ); 
	m_Object = obj; 
	m_Widget->setup( m_Object ); 
	m_Widget->updateEditor(); 
	ObjectViewsTable::bindView(m_Object, this);
	return m_Widget; 
}

Object * MethodBrowser::getObject() {return m_Object;}

bool MethodBrowser::standAloneEditorApplication() {return false;}

MethodBrowser::~MethodBrowser() {
	ObjectViewsTable::releaseView(m_Object, this);
}


// ---


MethodBrowserCustomButton::MethodBrowserCustomButton(int index, QWidget * parent )
 : QPushButton(parent) 
{
	this->index = index;
	QObject::connect(this, SIGNAL( clicked(void) ), this, SLOT( receiveClick(void) ) );
}

MethodBrowserCustomButton::MethodBrowserCustomButton(int index, const QString & text, QWidget * parent )
 : QPushButton(text, parent) 
{
	this->index = index;
	QObject::connect(this, SIGNAL( clicked(void) ), this, SLOT( receiveClick(void) ) );
}

MethodBrowserCustomButton::MethodBrowserCustomButton(int index, const QIcon & icon, const QString & text, QWidget * parent )
 : QPushButton(icon, text, parent) 
{
	this->index = index;
	QObject::connect(this, SIGNAL( clicked(void) ), this, SLOT( receiveClick(void) ) );
}

void MethodBrowserCustomButton::receiveClick() {
	emit methodClicked(index);
}


MethodBrowserWidget::MethodBrowserWidget(QWidget * parent) : QScrollArea(parent)
{
	setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
	setWidgetResizable(true);
	setFrameStyle(0);
	//resize( 240, 200 );
	
	frame = new QFrame(this);
	//QVBoxLayout *l = new QVBoxLayout(this);
	//l->setMargin(0);
	//l->setSpacing(0);
	//this->setLayout(l);
	layout = new QVBoxLayout(frame);
	layout->setMargin(0);
	layout->setSpacing(3);
	defaultHeight = 100;
	setWidget(frame);
}

MethodBrowserWidget::~MethodBrowserWidget(void)
{
}

void MethodBrowserWidget::setup( Object* obj )
{
	m_Object = obj;
	if (obj == NULL) return;

	const MetaClass *mc = obj->getInstanceClass();
	
	const card32 n = mc->getNumMethods();
	
	int h = 0;
	
	int index = 0; // real index, without invisible methods
	for (card32 i=0; i<n; i++) {
		const TimerClassMethod *tm = dynamic_cast<const TimerClassMethod*>(mc->getMethod(i));
		if (tm) {
			if (tm->getVisible()) {
				methods.push_back(tm);
				string name = tm->getName();
				TimerMethodWidget *widget =
					new TimerMethodWidget(frame, index, name.c_str(), tm->getDefaultIntervalMilliSec());
				index++;
				h += 33;
				widget->setMinimumHeight(33);
				widget->setMaximumHeight(33);
				QObject::connect( widget, SIGNAL( methodClicked(int) ), this, SLOT( methodClicked(int) ) );
				layout->addWidget( widget );
			}
		} else {
			const NoArgsClassMethod *m = dynamic_cast<const NoArgsClassMethod*>(mc->getMethod(i));
			if (m != NULL && m->getVisible()) {
				methods.push_back(m);
				string name = m->getName();
				MethodBrowserCustomButton *btn;
				name = string(" ") + name;
				if (m->getIcon() == "") {
					btn = new MethodBrowserCustomButton(index, QIcon(":/images/Method.png"), name.c_str(), frame);
				} else {
					btn = new MethodBrowserCustomButton(index, QIcon(m->getIcon().c_str()), name.c_str(), frame);
				}
				index++;
				if (m->getDescription() != "") {
					btn->setToolTip(m->getDescription().c_str());
				}
				//h += btn->height();
				h += 22;
				btn->setMinimumHeight(22);
				btn->setMaximumHeight(22);
				QObject::connect( btn, SIGNAL( methodClicked(int) ), this, SLOT( methodClicked(int) ) );
				layout->addWidget(btn);
			}
			layout->addStretch(1);
		}
	}
	
	h += 3*(((int)index)-1);
	//h = min(h, 200);
	defaultHeight = h;
	setMaximumHeight(h);
	resize(width(), h);
}

void MethodBrowserWidget::updateEditor() {}  // methods don't change...

void MethodBrowserWidget::methodClicked(int index) {
	STD_GLOBAL_EXCEPTION_HANDLER_START
	pAssert(index < (int)methods.size());
	if (index >= (int)methods.size()) return;
	methods[index]->call(m_Object);
	ObjectViewsTable::update(m_Object);
	STD_GLOBAL_EXCEPTION_HANDLER_END	
}


