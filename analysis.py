import pandas as pd


def read_text_file(file_path):
    with open(file_path, "r") as file:
        lines = file.readlines()
    return lines


def parse_results(log_lines):
    instance_numbers = set()
    greedy_profits = {}
    ip_profits = {}
    brkga_profits = {}

    for i in range(0, len(log_lines), 8):  # Assuming each block is 7 lines
        instance_line, config_line, profit_line = (
            log_lines[i + 2],
            log_lines[i + 3],
            log_lines[i + 4],
        )

        # Extract instance number
        instance_number = int(instance_line.split("_")[-1].split(".")[0])
        instance_numbers.add(instance_number)

        # Extract profit based on model
        profit = int(profit_line.split(": ")[-1])
        if "greedy" in config_line:
            greedy_profits[instance_number] = profit
        elif "ip" in config_line:
            ip_profits[instance_number] = profit
        elif "brkga" in config_line:
            brkga_profits[instance_number] = profit

    return instance_numbers, greedy_profits, ip_profits, brkga_profits


if __name__ == "__main__":
    log_lines = read_text_file("results.log")
    instance_numbers, greedy_profits, ip_profits, brkga_profits = parse_results(
        log_lines
    )
    df = pd.DataFrame(
        {
            "instance": list(instance_numbers),
            "greedy": [greedy_profits[i] for i in instance_numbers],
            "ip": [ip_profits[i] for i in instance_numbers],
            "brkga": [brkga_profits[i] for i in instance_numbers],
        }
    )
    df = df.sort_values(by="instance")
    average_ip = (df["brkga"] / df["ip"]).mean()
    average_greedy = (df["brkga"] / df["greedy"]).mean()
    print(f"Average IP: {average_ip}")
    print(f"Average Greedy: {average_greedy}")
