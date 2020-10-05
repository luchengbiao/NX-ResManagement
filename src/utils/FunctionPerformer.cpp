#include "FunctionPerformer.h"
#include <QThread>

FunctionPerformer::FunctionPerformer(QObject* parent)
    : QObject(parent)
	, my_thread_id_(QThread::currentThreadId())
{
	connect(this, &FunctionPerformer::Transfer, this, &FunctionPerformer::Perform);
}

void FunctionPerformer::PerformInMyThread(const std::function<void()>& func)
{
    if (QThread::currentThreadId() == my_thread_id_)
    {
		func();
	}
	else
	{
		emit Transfer(func);
	}
}

void FunctionPerformer::Perform(const std::function<void()>& func)
{
    func();
}

static FunctionPerformer* s_main_thread_performer_;

void FunctionPerformer::Init()
{
	qRegisterMetaType<std::function<void()>>("std::function<void()>");

	s_main_thread_performer_ = new FunctionPerformer(); // never delete
}

void FunctionPerformer::PerformInMainThread(const std::function<void()>& func)
{
	s_main_thread_performer_->PerformInMyThread(func);
}
