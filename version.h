#ifndef _VERSION_H
#define	_VERSION_H

//version log

/*1.0.0.0 2022-10-28T10:19*/
/*char-rnn-classification-c++版本工程启动
/*配套依赖:libtorch1.5 inopencv3.4.1 vs2017
/*关联参考:https://pytorch.org/tutorials/intermediate/char_rnn_classification_tutorial.html
*/

/*2022-10-28T18:06*/
/*完成工程初始化创建,对tools进行通用化升级*/

/*2022-10-31T17:25*/
/*完成文件遍历、读取等相关基础功能通用函数,使用模板作为函数参数通用化功能函数 _TOOLS_VERSION_*/

/*2022-11-04T11:30*/
/*完成character-level的RNN预测程序:训练和推理,RNN由最简单的两条线性函数组成,清晰的观察RNN隐藏状态以及隐藏状态如何工作*/

#define	CHAR_RNN_CLASSIFICATION_C_VERSION		1.0.0.0
#define	_WORD_EMBEDDING_VERSION_				1.0.0.0
#define	_ATTENTION_MECHANISM_VERSION_			1.0.0.0
#define	_RNN_KERNEL_VERSION_					1.0.0.0

#define	_TOOLS_VERSION_							2.0.0.1

#endif // !_VERSION_H
