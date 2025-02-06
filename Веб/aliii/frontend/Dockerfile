FROM node:16-alpine as build
WORKDIR /app

# Копируем package.json и yarn.lock
COPY package.json yarn.lock ./

# Устанавливаем зависимости
RUN yarn install

# Копируем исходный код
COPY . .

# Добавляем переменную окружения для отключения критических ошибок ESLint
ENV ESLINT_NO_DEV_ERRORS=true

# Запускаем сборку
RUN yarn build

# Производственный образ для Nginx
FROM nginx:alpine
COPY nginx.conf /etc/nginx/conf.d/default.conf
COPY --from=build /app/build /usr/share/nginx/html
EXPOSE 80
CMD ["nginx", "-g", "daemon off;"]
