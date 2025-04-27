

    const ws = new WebSocket(`ws://${location.hostname}:8086`);
    
	function enviar(tipo, valor) {
		if (ws.readyState === WebSocket.OPEN) {
			const mensaje = JSON.stringify({ t: tipo, v: valor });
			ws.send(mensaje);
			} 
		else {
			console.log("Conexión WebSocket no abierta.");
			}
		}

    ws.onopen = () => {
      document.getElementById("estado").innerText = "Conectado";
      };

	ws.onmessage = (event) => {
		const data = JSON.parse(event.data);
		const tipo = data.t;
		const valor = data.v;

		if (tipo === "temp") {
			const temp1 = (parseInt(valor.temp1) / 100).toFixed(1);
			const temp2 = (parseInt(valor.temp2) / 100).toFixed(1);
			const temp3 = (parseInt(valor.temp3) / 100).toFixed(1);
			if (document.getElementById("temp1")) document.getElementById("temp1").innerText = temp1 + " °C";
			if (document.getElementById("temp2")) document.getElementById("temp2").innerText = temp2 + " °C";
			if (document.getElementById("temp3")) document.getElementById("temp3").innerText = temp3 + " °C";
			return;
			}
		if (tipo === "s") {
			const sMeterValue = parseInt(valor.s);
			moverAgujaSMeter(sMeterValue);
			return;
			}
 
		if (tipo === "tx") {
			const pwrValue = parseInt(valor.p);
			const swrValue = parseFloat(valor.swr);
			moverAgujaPwr(pwrValue);
			const swrElement = document.getElementById("swr");
			if (swrElement) swrElement.innerText = swrValue.toFixed(1);
			return;
			}

		const el = document.getElementById(tipo);
		if (el) el.innerText = valor;

		if (tipo === "temp1" || tipo === "temp2" || tipo === "temp3") {
			const tempElement = document.getElementById(tipo);
			if (tempElement) tempElement.innerText = valor + " °C";
			}

		// Si el tipo es 'mode', actualizamos el texto del botón
		if (tipo === "mode") {
			const botonModo = document.getElementById("botonModo");
			if (botonModo) {
				// Actualiza el texto del botón dependiendo del valor recibido
				botonModo.innerText = valor; // Valor puede ser 'USB', 'LSB', 'AM', etc.
				}
			}

		// Actualizar VFO si es necesario
		if (tipo === "vfo") {
			const botonVfo = document.getElementById("botonVfo");
			if (botonVfo) {
				botonVfo.innerText = valor === "A" ? "VFO A" : "VFO B";
				}
			}

		// Si el tipo es 'spl', actualizamos el texto del botón SPLIT
		if (tipo === "spl") {
			const botonSplit = document.getElementById("botonSpl"); // Cambié a botonSpl
			if (botonSplit) {
				botonSplit.innerText = valor === "ON" ? "SPLIT ON" : "SPLIT OFF";
				if (valor === "ON") {
					botonSplit.style.backgroundColor = "orange"; // Naranja cuando está 'ON'
					} 
				else {
					botonSplit.style.backgroundColor = "gray"; // gris cuando está 'OFF'
					}
				}
			}
		// Si el tipo es 'rit', actualizamos el texto del botón RIT
		if (tipo === "rit") {
			const botonRit = document.getElementById("botonRit"); // Cambié a botonRit
			if (botonRit) {
				botonRit.innerText = valor === "ON" ? "RIT ON" : "RIT OFF";
				if (valor === "ON") {
					botonRit.style.backgroundColor = "green"; // Naranja cuando está 'ON'
					} 
				else {
					botonRit.style.backgroundColor = "gray"; // gris cuando está 'OFF'
					}
				}
			}
			
		// Si el tipo es 'fA', actualizar los dígitos de la frecuencia
		if (tipo === "fA") {
			let frecuencia = valor.toString();
			frecuencia = frecuencia.padStart(9, '0');
			// Actualizar los dígitos en el contenedor de fA_digits
			const digits = document.querySelectorAll('#fA_digits .digit');
			digits.forEach((digit, index) => {
				digit.innerText = frecuencia.charAt(index);
				});
			// También actualizar el valor completo de fA
			const fAElement = document.getElementById('fA');
			if (fAElement) {
				fAElement.innerText = frecuencia;
				}
			// Actualizamos la variable frecuenciaA
			frecuenciaA = frecuencia; // Sincronizamos la frecuencia con la variable global
			}
		};

	// Función para mover la aguja de potencia
	function moverAgujaPwr(valor) {
		const aguja = document.getElementById("agujaPwr");
		if (!aguja) return;
		const valorMaximo = 100; // 100% de potencia
		const anguloMin = -90;   // completamente a la izquierda
		const anguloMax = 90;    // completamente a la derecha
		const porcentaje = Math.min(Math.max(valor / valorMaximo, 0), 1);
		const angulo = anguloMin + porcentaje * (anguloMax - anguloMin);
		aguja.style.transform = `rotate(${angulo}deg)`;
		}

	// Función para mover la aguja de S-Meter
	function moverAgujaSMeter(valor) {
		const aguja = document.getElementById("agujaSMeter");
		if (!aguja) return;
		const angulo = (valor / 100) * 100 - 50; // De 0 a 100 → -50° a +50°
		aguja.style.transform = `rotate(${angulo}deg)`;
		aguja.style.transformOrigin = "bottom center";
		}

	// Obtener todos los dígitos de la frecuencia A
	const digits = document.querySelectorAll("#fA_digits .digit");
	// Variable que almacenará el valor actual de la frecuencia A (inicializada en 9 ceros)
	let frecuenciaA = "000000000";  // Frecuencia de 9 dígitos

	// Función para manejar el cambio de un dígito
	function handleWheelEvent(event) {
		event.preventDefault(); // Prevenir el comportamiento por defecto (scroll de la página)
		const target = event.target;
		if (!target.classList.contains("digit")) return; // Asegurarse de que el objetivo sea un dígito
		const index = parseInt(target.dataset.index, 10); // Obtener el índice del dígito
		let digitValue = parseInt(target.innerText, 10); // Obtener el valor actual del dígito
		// Determinar si la rueda del ratón fue hacia arriba o hacia abajo
		if (event.deltaY < 0) {
			digitValue = (digitValue + 1) % 10; // Incrementar el dígito, ciclo de 0 a 9
			} 
		else if (event.deltaY > 0) {
			digitValue = (digitValue - 1 + 10) % 10; // Decrementar el dígito, ciclo de 9 a 0
			}
		// Actualizar el dígito en la interfaz
		target.innerText = digitValue;
		frecuenciaA = frecuenciaA.substring(0, index) + digitValue + frecuenciaA.substring(index + 1);
		frecuenciaA = frecuenciaA.padStart(9, '0');
		// Enviar el valor de la frecuencia A al servidor (si es necesario)
		enviar('fA', frecuenciaA); // O lo que sea necesario para enviar al servidor
		}

	// Añadir los manejadores de eventos a cada dígito
	digits.forEach(digit => {
		digit.addEventListener("wheel", handleWheelEvent);
		});

//     -------------------------------

//     -------------------------------  

//    -------------------------------  


