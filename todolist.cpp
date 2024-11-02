#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <string>

class Task {
public:
    std::string description;
    int priority; // 1 = High, 2 = Medium, 3 = Low
    std::string dueDate;

    Task(const std::string& desc, int prio, const std::string& due)
        : description(desc), priority(prio), dueDate(due) {}

    void display() const {
        std::cout << "Task: " << description 
                  << " | Priority: " << priority 
                  << " | Due Date: " << dueDate << "\n";
    }
};

class ToDoListManager {
private:
    std::vector<Task> tasks;
    std::map<int, std::vector<Task>> categorizedTasks;

public:
    void addTask(const std::string& desc, int priority, const std::string& due) {
        Task newTask(desc, priority, due);
        tasks.push_back(newTask);
        categorizedTasks[priority].push_back(newTask);
    }

    void removeTask(int index) {
        if (index < 0 || index >= tasks.size()) {
            std::cout << "Invalid task index.\n";
            return;
        }
        int priority = tasks[index].priority;
        tasks.erase(tasks.begin() + index);
        auto& priorityTasks = categorizedTasks[priority];
        for (auto it = priorityTasks.begin(); it != priorityTasks.end(); ++it) {
            if (it->description == tasks[index].description) {
                priorityTasks.erase(it);
                break;
            }
        }
    }

    void viewTasks() const {
        if (tasks.empty()) {
            std::cout << "No tasks to display.\n";
            return;
        }
        for (int i = 0; i < tasks.size(); ++i) {
            std::cout << "[" << i << "] ";
            tasks[i].display();
        }
    }

    void viewTasksByPriority(int priority) const {
        if (categorizedTasks.count(priority) == 0 || categorizedTasks.at(priority).empty()) {
            std::cout << "No tasks with this priority.\n";
            return;
        }
        for (const auto& task : categorizedTasks.at(priority)) {
            task.display();
        }
    }

    void saveToFile(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file) {
            std::cout << "Error opening file.\n";
            return;
        }
        for (const auto& task : tasks) {
            file << task.description << "," << task.priority << "," << task.dueDate << "\n";
        }
        file.close();
    }

    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) {
            std::cout << "Error opening file.\n";
            return;
        }
        tasks.clear();
        categorizedTasks.clear();

        std::string line;
        while (std::getline(file, line)) {
            size_t pos1 = line.find(',');
            size_t pos2 = line.find(',', pos1 + 1);
            std::string desc = line.substr(0, pos1);
            int prio = std::stoi(line.substr(pos1 + 1, pos2 - pos1 - 1));
            std::string due = line.substr(pos2 + 1);

            addTask(desc, prio, due);
        }
        file.close();
    }
};

int main() {
    ToDoListManager manager;
    int choice;

    do {
        std::cout << "\n1. Add Task\n2. Remove Task\n3. View Tasks\n4. View Tasks by Priority\n5. Save to File\n6. Load from File\n0. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        if (choice == 1) {
            std::cin.ignore(); // Clear newline
            std::string description, dueDate;
            int priority;
            std::cout << "Enter task description: ";
            std::getline(std::cin, description);
            std::cout << "Enter priority (1=High, 2=Medium, 3=Low): ";
            std::cin >> priority;
            std::cin.ignore();
            std::cout << "Enter due date: ";
            std::getline(std::cin, dueDate);
            manager.addTask(description, priority, dueDate);

        } else if (choice == 2) {
            int index;
            std::cout << "Enter task index to remove: ";
            std::cin >> index;
            manager.removeTask(index);

        } else if (choice == 3) {
            manager.viewTasks();

        } else if (choice == 4) {
            int priority;
            std::cout << "Enter priority to view (1=High, 2=Medium, 3=Low): ";
            std::cin >> priority;
            manager.viewTasksByPriority(priority);

        } else if (choice == 5) {
            manager.saveToFile("tasks.txt");

        } else if (choice == 6) {
            manager.loadFromFile("tasks.txt");

        } else if (choice != 0) {
            std::cout << "Invalid choice. Try again.\n";
        }

    } while (choice != 0);

    std::cout << "Exiting To-Do List Manager.\n";
    return 0;
}
