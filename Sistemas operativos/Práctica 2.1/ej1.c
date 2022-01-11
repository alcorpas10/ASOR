int main() {
    if (setuid(0) == -1) {
        perror("Error");
    }
    return 1;
}