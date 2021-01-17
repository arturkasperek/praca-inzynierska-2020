echo "Running web listener"
docker run -d -v $(pwd):/app --network graphic-app-network --name web-listener node:14-alpine node /app/web-listener/main.js
echo "Running wine container"
docker run -d -it -v $(pwd):/app --rm --hostname="$(hostname)" --network graphic-app-network  --publish="3389:3389/tcp" --name docker-wine-container scottyhardy/docker-wine tail -f /dev/null
sleep 3
echo "Running broadcaster"
docker exec -d docker-wine-container /bin/bash -c "/app/start.sh"
echo "Waiting for broadcaster to be up"
docker run -v $(pwd):/app --network graphic-app-network node:14-alpine node /app/wait-for-broadcaster/main.js
echo "Running graphic app"
docker exec -d docker-wine-container /bin/bash -c "DISPLAY=:1 wine /app/graphic-app/GraphicApp.exe 30" &
echo "Waiting for graphic app to finish work"
docker run -v $(pwd):/app --network graphic-app-network node:14-alpine node /app/wait-for-graphic-app/main.js
docker logs web-listener
echo "Removing not needed containers"
docker rm -f docker-wine-container
docker rm -f web-listener
