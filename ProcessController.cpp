#include"ProcessController.h"

ProcessController::ProcessController()
{
}

void ProcessController::insert_process(string PID, Process p) {
	process_map.insert(make_pair(PID,p));
}

void ProcessController::destroy_process(string PID) {
	Process &p = get_process(PID);
	//��ȡ�ӽ���
	map<string, Process>& children = p.get_children();
	auto children_map_it = children.begin();
	while (children_map_it != children.end()) {
		//�ӽ��̱�������ӽ���
		process_map.erase(children_map_it->second.get_PID());
		++children_map_it;
	}
	//�ӽ��̱��������ǰ����
	process_map.erase(PID);
}

Process& ProcessController::get_process(string PID) {
	return (*(process_map.find(PID))).second;
}

void ProcessController::scheduler(ListController& list_controller) {
	if (list_controller.get_running_process()== "") {
		if (list_controller.ready_list[2].size() > 1) {//system���ȼ��еȴ�����
			list_controller.change_ready_to_running(Process::System);
		}
		else if (list_controller.ready_list[1].size() > 1) {//User���ȼ��еȴ�����
			list_controller.change_ready_to_running(Process::User);
		}
		else if(list_controller.ready_list[0].size() > 1) {//Init���ȼ��еȴ�����
			list_controller.change_ready_to_running(Process::Init);
		}
		else {
			//�ȴ����̶���Ϊ��
		}
		//������running���̵�״̬Ϊrunning
		Process & new_running_process = process_map.find(list_controller.get_running_process())->second;
		new_running_process.set_process_status(Process::running);
	}
}

void ProcessController::timeout(ListController& list_controller) {
	string running_process_id = list_controller.get_running_process();
	Process &running_process = (*(process_map.find(running_process_id))).second;
	//����ǰrunning���̵���ready����
	list_controller.change_running_to_list(running_process_id, running_process.get_priority(),true);
	running_process.set_process_status(Process::ready);
	scheduler(list_controller);
}