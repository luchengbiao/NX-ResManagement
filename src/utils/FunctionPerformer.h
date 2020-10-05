#ifndef FUNCTIONPERFORMER_H
#define FUNCTIONPERFORMER_H
#include <functional>
#include <QObject>

class FunctionPerformer : public QObject
{
    Q_OBJECT

public:
    explicit FunctionPerformer(QObject* parent = nullptr);

public:
	static void Init();
    static void PerformInMainThread(const std::function<void()>& func);

    void PerformInMyThread(const std::function<void()>& func);

Q_SIGNALS:
    void Transfer(const std::function<void()>& func);

private Q_SLOTS:
    void Perform(const std::function<void()>& func);

private:
	const Qt::HANDLE my_thread_id_;
};

#endif // FUNCTIONPERFORMER_H
