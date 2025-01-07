# Sử dụng image Ubuntu làm cơ sở
FROM gcc:11.4.0

# # Cài đặt GCC và các công cụ cần thiết
# RUN apt-get update && apt-get install -y gcc && apt-get clean

# Tạo thư mục làm việc trong container
WORKDIR /app

# Sao chép mã nguồn từ máy chủ vào container
COPY docker_test.c .

# Biên dịch chương trình
RUN gcc docker_test.c -o docker_test

# Lệnh mặc định để chạy chương trình
CMD ["./docker_test"]
